// GfxDebug.cpp
//
#include "Bus.h"
#include "GfxDebug.h"
#include "C6809.h"
#include "font8x8_system.h"


Byte GfxDebug::read(Word offset, bool debug)
{
    Byte data = 0xCC;
    switch (offset)
    {
    case DBG_BRK_ADDR + 0: data = reg_brk_addr >> 8;   break;
    case DBG_BRK_ADDR + 1: data = reg_brk_addr & 0xFF; break;
    case DBG_FLAGS:
        (_bIsDebugActive) ? reg_flags |= 0x80 : reg_flags &= ~0x80; // Enable
        (bSingleStep)     ? reg_flags |= 0x40 : reg_flags &= ~0x40; // Single-Step
        reg_flags &= ~0x20;     // zero for Clear all Breakpoints
        (mapBreakpoints[reg_brk_addr]) ? reg_flags |= 0x10 : reg_flags &= ~0x10;
        reg_flags &= ~0x08;     // FIRQ
        reg_flags &= ~0x04;     // IRQ
        reg_flags &= ~0x02;     // NMI
        reg_flags &= ~0x01;     // RESET

        data = reg_flags; 
        break;
    }
    // update and return
    Bus::Write(offset, data, true);
    return data;
}
void GfxDebug::write(Word offset, Byte data, bool debug)
{
    switch (offset)
    {
    case DBG_BRK_ADDR + 0: reg_brk_addr = (reg_brk_addr & 0x00ff) | (data << 8); break;
    case DBG_BRK_ADDR + 1: reg_brk_addr = (reg_brk_addr & 0xff00) | (data << 0); break;
    case DBG_FLAGS:
        reg_flags = data;
        (reg_flags & 0x80) ? _bIsDebugActive = true : _bIsDebugActive = false;
        (reg_flags & 0x40) ? bSingleStep = true : bSingleStep = false;
        if (reg_flags & 0x20)  cbClearBreaks();
        (reg_flags & 0x10) ? mapBreakpoints[reg_brk_addr] = true : mapBreakpoints[reg_brk_addr] = false;
        if (reg_flags & 0x08)   cbFIRQ();
        if (reg_flags & 0x04)   cbIRQ();
        if (reg_flags & 0x02)   cbNMI();
        if (reg_flags & 0x01)   cbReset();
        break;
    }
    // write statically
    Bus::Write(offset, data, true);
}

Word GfxDebug::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Debug Hardware Registers:");
    DisplayEnum("DBG_BEGIN",    nextAddr, "Start of Debug Hardware Registers");
    DisplayEnum("DBG_BRK_ADDR", nextAddr, "   (Word) Address of current breakpoint");
    nextAddr += 2;
    DisplayEnum("DBG_FLAGS",    nextAddr, "   (Byte) Debug Specific Hardware Flags:");
    nextAddr += 1;
    DisplayEnum("", 0, "    bit 7: Debug Enable");
    DisplayEnum("", 0, "    bit 6: Single Step Enable");
    DisplayEnum("", 0, "    bit 5: Clear All Breakpoints");
    DisplayEnum("", 0, "    bit 4: Update Breakpoint at DEBUG_BRK_ADDR");
    DisplayEnum("", 0, "    bit 3: FIRQ  (on low to high edge)");
    DisplayEnum("", 0, "    bit 2: IRQ   (on low to high edge)");
    DisplayEnum("", 0, "    bit 1: NMI   (on low to high edge)");
    DisplayEnum("", 0, "    bit 0: RESET (on low to high edge)");
    DisplayEnum("DBG_END", nextAddr, "End Debug Registers");

    return nextAddr - old_addr;
}

void GfxDebug::OnInit() {}
void GfxDebug::OnQuit() {}

void GfxDebug::OnActivate()
{
    // debug layer texture size
    _tex_width = m_gfx->_base_texture_width * 4;
    _tex_height = _tex_width / m_gfx->_aspect;

    // create the glyph textures 
    // printf("glyph_textures.size(): %d\n", (int)glyph_textures.size());
    _onWindowResize();

    // create the working debug layer texture
    _debug_texture = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_ARGB4444,
        SDL_TEXTUREACCESS_TARGET, _tex_width, _tex_height);
    SDL_SetTextureBlendMode(_debug_texture, SDL_BLENDMODE_BLEND);
}
void GfxDebug::OnDeactivate()
{
    // printf("GfxDebug::OnDeactivate()\n");
    // destroy the debug layer texture
    if (_debug_texture)
    {
        SDL_DestroyTexture(_debug_texture);
        _debug_texture = nullptr;
    }
    // destroy the glyph textures
    for (auto& a : glyph_textures)
        SDL_DestroyTexture(a);
    glyph_textures.clear();

    // printf("glyph_textures.size(): %d\n", (int)glyph_textures.size());
}

void GfxDebug::_onWindowResize()
{
    // For whatever reason, when the viewing window gets resized, 
    //  the glyphs get corrupted. This is a hacky workaround solution is to recreate
    //  them when this happens. 

    // destroy the glyph textures
    for (auto& a : glyph_textures)
        SDL_DestroyTexture(a);
    glyph_textures.clear();

    // recreate the glyphs
    if (glyph_textures.size() == 0)
    {
        for (int t = 0; t < 256; t++)
        {
            SDL_Texture* glyph = SDL_CreateTexture(m_gfx->_renderer, SDL_PIXELFORMAT_RGBA4444,
                SDL_TEXTUREACCESS_TARGET, 8, 8);
            SDL_SetRenderTarget(m_gfx->_renderer, glyph);
            SDL_SetTextureBlendMode(glyph, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 0);
            SDL_RenderClear(m_gfx->_renderer);
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                {
                    Byte bitMask = 1 << (7 - x);
                    if (font8x8_system[t][y] & bitMask)
                    {
                        SDL_SetRenderDrawColor(m_gfx->_renderer, 255, 255, 255, 255);
                        SDL_RenderDrawPoint(m_gfx->_renderer, x, y);
                    }
                }
            }
            glyph_textures.push_back(glyph);
        }
    }
}

void GfxDebug::OnEvent(SDL_Event* evnt)
{
    switch (evnt->type)
    {
        case SDL_KEYDOWN: 
        {
            // ****************************
            // * Debugger Specific Events *
            // ****************************

            SDL_Keymod km = SDL_GetModState();
            if (km & KMOD_ALT)
            {
                if (evnt->key.keysym.sym == SDLK_d)
                {
                    // _bIsDebugActive = !_bIsDebugActive;
                    Byte data = Bus::Read(DBG_FLAGS);
                    data ^= 0x80;
                    Bus::Write(DBG_FLAGS, data);
                    bIsCursorVisible = false;
                    bMouseWheelActive = false;
                    if (_bIsDebugActive) // enable the cursor during debug
                    {
                        Byte data = Bus::Read(CSR_FLAGS);
                        data |= 0x20;
                        Bus::Write(CSR_FLAGS, data);
                    }
                }
                if (evnt->key.keysym.sym == SDLK_r)
                {
                    bSingleStep = !bSingleStep;
                    bMouseWheelActive = false;
                }
            }
            // perform debug enabled specific events
            if (_bIsDebugActive)
            {
                if (evnt->key.keysym.sym == SDLK_ESCAPE)
                    bIsCursorVisible = false;
                if (bIsCursorVisible)
                {
                    if (evnt->key.keysym.sym == SDLK_LEFT || evnt->key.keysym.sym == SDLK_BACKSPACE)
                        if (csr_x > 1)
                            while (!CoordIsValid(--csr_x, csr_y));
                    if (evnt->key.keysym.sym == SDLK_RIGHT)
                        if (csr_x < 28)
                            while (!CoordIsValid(++csr_x, csr_y));
                    if (evnt->key.keysym.sym == SDLK_UP)
                    {
                        if (csr_y == 1)			mem_bank[0] -= 8;
                        else if (csr_y == 11)	mem_bank[1] -= 8;
                        else if (csr_y == 21)	mem_bank[2] -= 8;
                        else if (csr_y > 1)		while (!CoordIsValid(csr_x, --csr_y));
                    }
                    if (evnt->key.keysym.sym == SDLK_DOWN)
                    {
                        if (csr_y == 9)			mem_bank[0] += 8;
                        else if (csr_y == 19)	mem_bank[1] += 8;
                        else if (csr_y == 29)	mem_bank[2] += 8;
                        else if (csr_y < 30)	while (!CoordIsValid(csr_x, ++csr_y));
                    }
                    if (evnt->key.keysym.sym == SDLK_RETURN)
                        bIsCursorVisible = false;
                }
                // SPACE advances single step
                if (evnt->key.keysym.sym == SDLK_SPACE)
                {
                    bSingleStep = true;
                    bIsStepPaused = false;
                    nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
                    bMouseWheelActive = false;
                }
             
            }
            break;
        } // SDL_KEYDOWN

        case SDL_MOUSEWHEEL:
            mouse_wheel = evnt->wheel.y;
            break;

        // case SDL_KEYUP:
        // {
        //     // toggle fullscreen/windowed 
        //     if (evnt->key.keysym.sym == SDLK_RETURN)
        //     {
        //         if (SDL_GetModState() & KMOD_ALT)
        //         {
        //             Byte data = Bus::Read(DSP_EXT);
        //             if (m_gfx->_windowed)
        //                 data &= ~0x01;
        //             else
        //                 data |= 0x01;
        //             Bus::Write(DSP_EXT, data);
        //         }
        //     }    
        //     break;        
        // }
    }
}

void GfxDebug::OnUpdate(float fElapsedTime)
{
    // wait until the cpu thread is up and running
    if (Bus::m_cpu == nullptr)
        return;
    // do nothing if the debugger is not active
    if (!_bIsDebugActive)
        return;

    // frame rate limit the debug layer updates
    const float delay = 1.0f / 30.0f;
    static float delayAcc = fElapsedTime;
    delayAcc += fElapsedTime;
    if (delayAcc >= delay)
    {
        delayAcc -= delay;

        // clear the debug layer texture
        SDL_SetRenderTarget(m_gfx->_renderer, _debug_texture);
        SDL_SetRenderDrawColor(m_gfx->_renderer, 0, 0, 0, 160);   // debug layer background color
        SDL_RenderClear(m_gfx->_renderer);

        // call update functions
        MouseStuff();
        KeyboardStuff();
        DumpMemory(1, 1, mem_bank[0]);
        DumpMemory(1, 11, mem_bank[1]);
        DumpMemory(1, 21, mem_bank[2]);
        DrawCpu(39, 1);
        DrawCode(39, 6);
        DrawButtons();
        HandleButtons();
        DrawBreakpoints();
        if (!EditRegister(fElapsedTime))
            DrawCursor(fElapsedTime);

        // instruction text
        OutText(1, 31, "[SPACE]    Step", 128, 128, 128);
        OutText(1, 32, "[ALT-X]    Quit", 128, 128, 128);
        OutText(1, 33, "[ALT-D] ~ Debug", 128, 128, 128);
        OutText(1, 34, "[ALT-R] RunStop", 128, 128, 128);
        OutText(1, 35, "[ALT-ENTER] Toggles between Fullscreen and Windowed", 128, 128, 128);

        // clean up
        SDL_SetRenderTarget(m_gfx->_renderer, NULL);
    }
}

void GfxDebug::OnRender()
{
    // do nothing if the debugger is not active
    if (!_bIsDebugActive)
        return;

    SDL_SetRenderTarget(m_gfx->_renderer, NULL);

    // build the destination rectangle according to current aspect ratio
    int ww = m_gfx->_window_width;
    int wh = m_gfx->_window_height;
    float fh = (float)m_gfx->_window_height;
    float fw = fh * m_gfx->_aspect;

    if (fw > ww)
    {
        fw = ww;
        fh = fw / m_gfx->_aspect;
    }
    SDL_Rect dest =
    {
        int(m_gfx->_window_width / 2 - (int)fw / 2),
        int(wh / 2 - (int)fh / 2),
        (int)fw,
        (int)fh
    };
    // render the mouse layer texture
    SDL_RenderCopy(m_gfx->_renderer, _debug_texture, NULL, &dest);
}

void GfxDebug::OutGlyph(int col, int row, Byte glyph, Byte red, Byte grn, Byte blu, bool bDropShadow)
{
    SDL_Rect dst = { col * 8, row * 8, 8, 8 };
    SDL_SetRenderTarget(m_gfx->_renderer, _debug_texture);
    if (bDropShadow)
    {
        SDL_Rect drop = { dst.x + 1, dst.y + 1, dst.w, dst.h };
        SDL_SetTextureColorMod(glyph_textures[glyph], 0, 0, 0);
        SDL_RenderCopy(m_gfx->_renderer, glyph_textures[glyph], NULL, &drop);

    }
    SDL_SetTextureColorMod(glyph_textures[glyph], red, grn, blu);
    SDL_RenderCopy(m_gfx->_renderer, glyph_textures[glyph], NULL, &dst);
}

int GfxDebug::OutText(int col, int row, std::string text, Byte red, Byte grn, Byte blu, bool bDropshadow)
{
    int pos = 0;
    for (auto& a : text)
        OutGlyph(col++, row, text[pos++], red, grn, blu, bDropshadow);
    return pos;
}

std::string GfxDebug::_hex(Uint32 n, Uint8 d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
}

void GfxDebug::DumpMemory(int col, int row, Word addr)
{
    const bool use_debug_read = false;
    int line = 0;
    for (int ofs = addr; ofs < addr + 0x48; ofs += 8)
    {
        int c = col;
        std::string out = _hex(ofs, 4) + " ";
        if (use_debug_read)
            for (int b = 0; b < 8; b++)
                out += _hex(Bus::Read(ofs + b, true), 2) + " ";
        else
            for (int b = 0; b < 8; b++)
                out += _hex(Bus::Read(ofs + b), 2) + " ";

        c += OutText(col, row + line, out.c_str(), 224, 224, 255);

        bool characters = true;
        if (characters)
        {
            for (int b = 0; b < 8; b++)
            {
                Byte data;
                if (use_debug_read)
                    data = Bus::Read(ofs + b, true);
                else
                    data = Bus::Read(ofs + b);
                OutGlyph(c++, row + line, data, 160, 160, 255);
            }
        }
        line++;
    }
}


void GfxDebug::DrawCpu(int x, int y)
{
    C6809* cpu = Bus::Inst().m_cpu;

    int RamX = x, RamY = y;
    // Condition Codes
    RamX += OutText(RamX, RamY, "CC($", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getCC(), 2).c_str(), 255, 255, 64);
    RamX += OutText(RamX, RamY, "): ", 64, 255, 64);
    if (cpu->getCC_E())		RamX += OutText(RamX, RamY, "E", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "e", 64, 255, 64);
    if (cpu->getCC_F())		RamX += OutText(RamX, RamY, "F", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "f", 64, 255, 64);
    if (cpu->getCC_H())		RamX += OutText(RamX, RamY, "H", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "h", 64, 255, 64);
    if (cpu->getCC_I())		RamX += OutText(RamX, RamY, "I", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "i", 64, 255, 64);
    if (cpu->getCC_N())		RamX += OutText(RamX, RamY, "N", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "n", 64, 255, 64);
    if (cpu->getCC_Z())		RamX += OutText(RamX, RamY, "Z", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "z", 64, 255, 64);
    if (cpu->getCC_V())		RamX += OutText(RamX, RamY, "V", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "v", 64, 255, 64);
    if (cpu->getCC_C())		RamX += OutText(RamX, RamY, "C", 255, 255, 64);
    else RamX += OutText(RamX, RamY, "c", 64, 255, 64);
    RamX = x; RamY++;	// carraige return(ish)

    // D = (A<<8) | B & 0x00FF
    RamX += OutText(RamX, RamY, "D:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getD(), 4), 255, 255, 64);
    RamX += OutText(RamX, RamY, " (A:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getA(), 2), 255, 255, 64);
    RamX += OutText(RamX, RamY, " B:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getB(), 2), 255, 255, 64);
    RamX += OutText(RamX, RamY, ")", 64, 255, 64);
    RamX = x; RamY++;	// carraige return(ish)

    // X
    RamX += OutText(RamX, RamY, " X:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getX(), 4), 255, 255, 64);
    // Y
    RamX += OutText(RamX, RamY, " Y:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getY(), 4), 255, 255, 64);
    // U
    RamX += OutText(RamX, RamY, " U:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getU(), 4), 255, 255, 64);
    RamX = x; RamY++;	// carraige return(ish)
    // PC
    RamX += OutText(RamX, RamY, "PC:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getPC(), 4), 255, 255, 64);
    // S
    RamX += OutText(RamX, RamY, " S:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getS(), 4), 255, 255, 64);
    // DP
    RamX += OutText(RamX, RamY, " DP:$", 64, 255, 64);
    RamX += OutText(RamX, RamY, _hex(cpu->getDP(), 2), 255, 255, 64);
    RamX = x; RamY++;	// carraige return(ish)
}

void GfxDebug::DrawCode(int col, int row)
{
    C6809* cpu = Bus::Inst().m_cpu;

    std::string code = "";
    int line = 0;
    Word next = 0;
    sDisplayedAsm.clear();

    if (bMouseWheelActive)
    {
        Word cpu_PC = cpu->getPC();
        Word offset = cpu_PC + mousewheel_offset;
        while (line < 24)
        {
            if (offset < cpu_PC)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 208, 32, 32);
                else
                    OutText(col, row + line, code, 96, 128, 32);
                line++;
            }
            if (offset == cpu_PC)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 255, 208, 128);
                else
                    OutText(col, row + line, code, 255, 255, 0);
                line++;
            }
            if (offset > cpu_PC)
            {
                bool atBreak = false;
                if (mapBreakpoints[offset])	atBreak = true;
                sDisplayedAsm.push_back(offset);
                code = cpu->disasm(offset, offset);
                if (atBreak)
                    OutText(col, row + line, code, 128, 64, 32);
                else
                    OutText(col, row + line, code, 128, 192, 64);
                line++;
            }

        }

    }
    else
    {
        // draw the last several lines
        for (auto& a : asmHistory)
        {
            if (a != cpu->getPC())
            {
                bool atBreak = false;
                if (mapBreakpoints[a])	atBreak = true;
                sDisplayedAsm.push_back(a);
                code = cpu->disasm(a, next);
                if (atBreak)
                    OutText(col, row + line++, code, 208, 32, 32);
                else
                    OutText(col, row + line++, code, 128, 128, 128);
            }
        }
        // draw the current line
        sDisplayedAsm.push_back(cpu->getPC());
        code = cpu->disasm(cpu->getPC(), next);
        if (mapBreakpoints[cpu->getPC()])
            OutText(col, row + line++, code, 255, 96, 32);
        else
            OutText(col, row + line++, code, 255, 255, 255);
        // create a history of addresses to display in the future
        static Word last = cpu->getPC();
        if (last != cpu->getPC())
        {
            last = cpu->getPC();
            asmHistory.push_back(cpu->getPC());
            while (asmHistory.size() > 12)
                asmHistory.pop_front();
        }
        // draw the next several future lines
        while (line < 24)
        {
            bool atBreak = false;
            if (mapBreakpoints[next])	atBreak = true;
            sDisplayedAsm.push_back(next);
            code = cpu->disasm(next, next);
            if (atBreak)
                OutText(col, row + line++, code, 208, 64, 32);
            else
                OutText(col, row + line++, code, 192, 192, 128);
        }
    }
}


void GfxDebug::DrawButtons()
{
    C6809* cpu = Bus::Inst().m_cpu;

    // change the run/stop according to the single step state
    if (bSingleStep)
    {
        vButton[5].text = " RUN!";
        vButton[5].x_min = 17;
        vButton[5].clr_index = 0xB;
    }
    else {
        vButton[5].text = " STOP";
        vButton[5].x_min = 17;
        vButton[5].clr_index = 0xA;
    }
    if (bEditingBreakpoint)
    {
        vButton[9].text = "[$";
        vButton[9].text += _hex(new_breakpoint, 4);
        vButton[9].text += "]";
        vButton[9].x_min = 48;
        vButton[9].clr_index = 0xC;
    }
    else
    {
        vButton[9].text = "ADD BRK";
        vButton[9].x_min = 48;
        vButton[9].clr_index = 0xC;
    }
    // draw the buttons
    for (auto& a : vButton)
    {
        int x1 = a.x_min;
        int x2 = a.x_max;
        int y = a.y_pos;
        // background
        int p = a.clr_index % 16;
        for (int x = x1; x <= x2; x++)
        {
            Byte r = (m_gfx->red(p) & 0x0f) << 4 | (m_gfx->red(p) & 0x0f);
            Byte g = (m_gfx->grn(p) & 0x0f) << 4 | (m_gfx->grn(p) & 0x0f);
            Byte b = (m_gfx->blu(p) & 0x0f) << 4 | (m_gfx->blu(p) & 0x0f);
            OutGlyph(x, y, 0x8f, r, g, b, true);
            //OutGlyph(x, y, 0x8f, 255, m_gfx->grn(p), m_gfx->blu(p), true);
        }
        // foreground
        int sX = x1 + ((x2 - x1) / 2) - (int)a.text.size() / 2;
        OutText(sX, y, a.text, 0, 0, 0, false);
    }
}

void GfxDebug::DrawBreakpoints()
{
    C6809* cpu = Bus::Inst().m_cpu;

    int x = 56, y = 33;		// y <= 38
    Uint8 ci = 0x0C;

    // build a vector of active breakpoints
    std::vector<Word> breakpoints;
    for (auto& bp : mapBreakpoints)
        if (bp.second)
            breakpoints.push_back(bp.first);
    // standard display
    if (breakpoints.size() < 8)
    {
        for (int t = 0; t < breakpoints.size(); t++)
        {
            std::string strBkpt = "[$";
            strBkpt += _hex(breakpoints[t], 4);
            strBkpt += "]";
            OutText(x, y, strBkpt, 255, 32, 32, true);
            y++;
        }
    }
    // oversized, mousewheel scrollable, display
    else
    {
        if (mw_brk_offset < 0)							mw_brk_offset = 0;
        if (mw_brk_offset + 7 > breakpoints.size())		mw_brk_offset = breakpoints.size() - 7;

        int index = mw_brk_offset;
        for (int t = 0; t < 7; t++)
        {
            std::string strBkpt;

            if (t == 0 && mw_brk_offset > 0)
                strBkpt = "[ ... ]";
            else if (t == 6 && index != breakpoints.size() - 1)
                strBkpt = "[ ... ]";
            else
            {
                strBkpt = "[$";
                strBkpt += _hex(breakpoints[index], 4);
                strBkpt += "]";
            }
            OutText(x, y, strBkpt, 255, 32, 32, true);
            y++;
            index++;
        }
    }
}

void GfxDebug::HandleButtons()
{
    C6809* cpu = Bus::Inst().m_cpu;
    int mx, my;
    _correctMouseCoords(mx, my);
    Uint32 btns = SDL_GetRelativeMouseState(NULL, NULL);

    static bool last_LMB = false;
    if (btns & 1 && !last_LMB)
    {
        for (auto& a : vButton)
        {
            if (my == a.y_pos && mx >= a.x_min && mx <= a.x_max)
            {
                //printf("Click %s\n", a.text.c_str());
                if (a.cbFunction)	(this->*a.cbFunction)();
            }
        }
    }
    last_LMB = (btns & 1);
}


bool GfxDebug::SingleStep()
{
    // do nothing if singlestep is disabled
    if (!bSingleStep)
        return true;
    // wait for space
    if (bIsStepPaused)
    {
        // bIsStepPaused = bSingleStep;
        return false;
    }
    return true;
}

void GfxDebug::ContinueSingleStep() {
    C6809* cpu = Bus::Inst().m_cpu;
    // if breakpoint reached... enable singlestep
    if (mapBreakpoints[cpu->getPC()] == true)
    {
        _bIsDebugActive = true;
        bSingleStep = true;
    }
    // continue from paused state?
    bIsStepPaused = bSingleStep;
}

bool GfxDebug::CoordIsValid(int x, int y)
{
    if (y > 0 && y < 30 && y != 10 && y != 20)
    {
        // at an address
        if (x > 0 && x < 5)
        {
            csr_at = CSR_AT::CSR_AT_ADDRESS;
            return true;
        }
        // at a data entry
        if (x > 5 && x < 29 && (x + 1) % 3)
        {
            csr_at = CSR_AT::CSR_AT_DATA;
            return true;
        }
    }
    return false;
}




bool GfxDebug::EditRegister(float fElapsedTime)
{
    if (nRegisterBeingEdited.reg == EDIT_REGISTER::EDIT_NONE)
        return false;

    C6809* cpu = Bus::Inst().m_cpu;

    static float delay = 0.0625f;
    static float delayAcc = fElapsedTime;
    static int ci = 9;
    delayAcc += fElapsedTime;
    std::string ch = " ";

    if (delayAcc > delay + fElapsedTime)
    {
        delayAcc -= delay;
        ci++;
        if (ci > 15)	ci = 9;
    }
    OutGlyph(csr_x, csr_y, 0x8f, rand() % 256, rand() % 256, rand() % 256);

    Word data = 0;
    switch (nRegisterBeingEdited.reg) {
    case EDIT_CC:	data = (Word)cpu->getCC() << 8; break;
    case EDIT_D:	data = cpu->getD(); break;
    case EDIT_A:	data = (Word)cpu->getA() << 8; break;
    case EDIT_B:	data = (Word)cpu->getB() << 8; break;
    case EDIT_X:	data = cpu->getX(); break;
    case EDIT_Y:	data = cpu->getY(); break;
    case EDIT_U:	data = cpu->getU(); break;
    case EDIT_PC:	data = cpu->getPC(); bSingleStep = true;  break;
    case EDIT_S:	data = cpu->getS(); break;
    case EDIT_DP:	data = (Word)cpu->getDP() << 8; break;
    case EDIT_BREAK: data = new_breakpoint; break;
    }
    Byte digit = csr_x - nRegisterBeingEdited.x_min;
    Byte num = 0;
    if (digit == 0)	num = ((data & 0xf000) >> 12);
    if (digit == 1)	num = ((data & 0x0f00) >> 8);
    if (digit == 2)	num = ((data & 0x00f0) >> 4);
    if (digit == 3)	num = ((data & 0x000f) >> 0);
    ch = _hex(num, 1);
    OutGlyph(csr_x, csr_y, ch[0], 0, 0, 0, false);

    // respond to numeric keys
    SDL_Keycode hx[] = { SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
                         SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
                         SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
                         SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F };
    char k[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    char d[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    static bool state[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    const Uint8* keybfr = SDL_GetKeyboardState(NULL);
    Word n = 0;
    bool bKeyPressed = false;
    for (int t = 0; t < 16; t++)
    {
        if (state[t] == 0 && keybfr[hx[t]])
        {
            n = d[t];
            bKeyPressed = true;
            state[t] = 1;
        }
        // reset the key
        if (state[t] == 1 && !keybfr[hx[t]])	state[t] = 0;
    }

    if (bKeyPressed)
    {
        //printf("pressed\n");
        if (digit == 0)		data = (data & 0x0fff) | (n << 12);
        if (digit == 1)		data = (data & 0xf0ff) | (n << 8);
        if (digit == 2)		data = (data & 0xff0f) | (n << 4);
        if (digit == 3)		data = (data & 0xfff0) | (n << 0);

        switch (nRegisterBeingEdited.reg) {
        case EDIT_CC:	cpu->setCC(data >> 8);  break;
        case EDIT_D:	cpu->setD(data);		break;
        case EDIT_A:	cpu->setA(data >> 8);	break;
        case EDIT_B:	cpu->setB(data >> 8);	break;
        case EDIT_X:	cpu->setX(data);		break;
        case EDIT_Y:	cpu->setY(data);		break;
        case EDIT_U:	cpu->setU(data);		break;
        case EDIT_PC:	cpu->setPC(data);		bMouseWheelActive = false;  break;
        case EDIT_S:	cpu->setS(data);		break;
        case EDIT_DP:	cpu->setDP(data >> 8);  break;
        case EDIT_BREAK: new_breakpoint = data; break;
        }
        if (csr_x < nRegisterBeingEdited.x_max)
            csr_x++;
    }
    SDL_Keycode ex[] = { SDL_SCANCODE_LEFT , SDL_SCANCODE_RIGHT , SDL_SCANCODE_RETURN , SDL_SCANCODE_ESCAPE };
    static bool bEx[] = { 0, 0, 0, 0 };
    for (int t = 0; t < 4; t++)
    {
        if (bEx[t] == 0 && keybfr[ex[t]])
        {
            // left
            if (ex[t] == SDL_SCANCODE_LEFT)
                if (csr_x > nRegisterBeingEdited.x_min)
                    csr_x--;
            // right
            if (ex[t] == SDL_SCANCODE_RIGHT)
                if (csr_x < nRegisterBeingEdited.x_max)
                    csr_x++;
            // enter updates and saves the register
            if (ex[t] == SDL_SCANCODE_RETURN || ex[t] == SDL_SCANCODE_ESCAPE)
            {
                if (nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_BREAK)
                {
                    mapBreakpoints[new_breakpoint] = true;
                    nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_NONE;
                    bEditingBreakpoint = false;
                }
                nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_NONE;
            }
            bEx[t] = 1;
        }
        else if (bEx[t] == 1 && !keybfr[ex[t]])
            bEx[t] = 0;
    }
    return true;
}

void GfxDebug::DrawCursor(float fElapsedTime)
{
    if (!bIsCursorVisible)	return;
    
    C6809* cpu = Bus::Inst().m_cpu;
    std::string ch = " ";
    // output the cursor
    OutGlyph(csr_x, csr_y, 0x8f, rand() % 256, rand() % 256, rand() % 256, true);
    // which character is under the cursor?
    switch (csr_at)
    {
    case CSR_AT::CSR_AT_ADDRESS:
    {
        Word addr = 0;
        if (csr_y < 10) addr = mem_bank[0] + ((csr_y - 1) * 8);
        else if (csr_y < 20) addr = mem_bank[1] + ((csr_y - 11) * 8);
        else if (csr_y < 30) addr = mem_bank[2] + ((csr_y - 21) * 8);
        Byte digit = csr_x - 1;
        Byte num = 0;
        if (digit == 0)	num = ((addr & 0xf000) >> 12);
        if (digit == 1)	num = ((addr & 0x0f00) >> 8);
        if (digit == 2)	num = ((addr & 0x00f0) >> 4);
        if (digit == 3)	num = ((addr & 0x000f) >> 0);
        ch = _hex(num, 1);
        break;
    }
    case CSR_AT::CSR_AT_DATA:
    {
        Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 8);
        Byte digit = ((csr_x + 1) % 3) - 1;
        Byte num = 0;
        Word addr = mem_bank[0];
        if (csr_y > 10 && csr_y < 20) { ofs -= 80; addr = mem_bank[1]; }

        if (csr_y > 20) { ofs -= 160; addr = mem_bank[2]; }


        Byte data = Bus::Read(addr + ofs, true);
        if (digit == 0) num = (data & 0xf0) >> 4;
        if (digit == 1) num = (data & 0x0f) >> 0;
        ch = _hex(num, 1);
        break;
    }

    }
    // display the reversed character
    OutGlyph(csr_x, csr_y, ch[0], 0, 0, 0, false);
}

void GfxDebug::_correctMouseCoords(int& mx, int& my)
{ 
    SDL_GetMouseState(&mx, &my);
    int ww, wh;
    SDL_GetWindowSize(m_gfx->_window, &ww, &wh);

    float fh = (float)wh;
    float fw = fh * m_gfx->_aspect;
    if (fw > ww)
    {
        fw = (float)ww;
        fh = fw / m_gfx->_aspect;
    }
    int tw = _tex_width;
    int th = _tex_height;
    SDL_Rect dest = { int(ww / 2 - (int)fw / 2), int(wh / 2 - (int)fh / 2), (int)fw, (int)fh };
    float w_aspect = (float)dest.w / tw;  
    float h_aspect = (float)dest.h / th;  
    mx = int((mx / w_aspect) - (dest.x / w_aspect));
    my = int((my / h_aspect) - (dest.y / h_aspect));
    mx /= 8;
    my /= 8;

    // printf("TW:%d  TH:%d   MX:%d   MY:%d\n",
    //    tw,
    //    th,
    //    mx, my);

    return;
}

void GfxDebug::MouseStuff()
{
    C6809* cpu = Bus::Inst().m_cpu;
    int mx, my;
    _correctMouseCoords(mx, my);
    Uint32 btns = SDL_GetRelativeMouseState(NULL, NULL);

    // mouse wheel
    if (mouse_wheel)
    {
        // scroll memory banks
        if (mx > 0 && mx < 29)
        {
            if (my > 0 && my < 10)	mem_bank[0] -= mouse_wheel * 8;
            if (my > 10 && my < 20)	mem_bank[1] -= mouse_wheel * 8;
            if (my > 20 && my < 30)	mem_bank[2] -= mouse_wheel * 8;
            bIsCursorVisible = false;
        }
        // Scroll the Code
        if (mx > 38 && mx < 64 && my > 5 && my < 30)
        {
            if (bMouseWheelActive == false)
            {
                mousewheel_offset = -25;
                bMouseWheelActive = true;
            }
            bSingleStep = true;	// scrollwheel enters into single step mode
            nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
            mousewheel_offset -= mouse_wheel * 1;		// slow scroll
            if (SDL_GetModState() & KMOD_CTRL)	// is CTRL down?
                mousewheel_offset -= mouse_wheel * 3;	// faster scroll			
        }
        // scroll the break point display window (bottom right corner)
        if (mx >= 55 && my >= 33)
        {
            // printf("mouse_wheel: %d\n", mouse_wheel);
            mw_brk_offset -= mouse_wheel;
        }

        // reset the wheel
        mouse_wheel = 0;
    }

    // left mouse button clicked?
    static bool last_LMB = false;
    if ((btns & 1) && !last_LMB)
    {
        // left-clicked on breakpoint
        if (mx >= 55 && my >= 33)
        {
            int index = (my - 33) + mw_brk_offset;
            // build a vector of active breakpoints
            std::vector<Word> breakpoints;
            for (auto& bp : mapBreakpoints)
                if (bp.second)
                    breakpoints.push_back(bp.first);
            if (index < breakpoints.size())
                printf("LEFT CLICK: $%04X\n", breakpoints[index]);
            //mapBreakpoints[breakpoints[index]] = false;
        }

        // click to select
        if (btns & 1)
        {
            if (CoordIsValid(mx, my))
            {
                csr_x = mx;
                csr_y = my;
                bIsCursorVisible = true;
            }
            else
                bIsCursorVisible = false;
            //printf("MX:%d  MY:%d\n", mx, my);
        }
        // condition code clicked?
        if (my == 1)
        {
            if (mx == 48) cpu->setCC_E(!cpu->getCC_E());
            if (mx == 49) cpu->setCC_F(!cpu->getCC_F());
            if (mx == 50) cpu->setCC_H(!cpu->getCC_H());
            if (mx == 51) cpu->setCC_I(!cpu->getCC_I());
            if (mx == 52) cpu->setCC_N(!cpu->getCC_N());
            if (mx == 53) cpu->setCC_Z(!cpu->getCC_Z());
            if (mx == 54) cpu->setCC_V(!cpu->getCC_V());
            if (mx == 55) cpu->setCC_C(!cpu->getCC_C());
        }
        // Register Clicked?
        bool bFound = false;
        for (auto& a : register_info)
        {
            if (a.y_pos == my && mx >= a.x_min && mx <= a.x_max)
            {
                // begin editing a register
                nRegisterBeingEdited.reg = a.reg;
                nRegisterBeingEdited.value = a.value;
                nRegisterBeingEdited.y_pos = a.y_pos;
                nRegisterBeingEdited.x_min = a.x_min;
                nRegisterBeingEdited.x_max = a.x_max;
                csr_x = mx;
                csr_y = my;
                bFound = true;
            }
        }
        if (!bFound)
            nRegisterBeingEdited.reg = EDIT_NONE;
        // left-click on code line toggles breakpoint
        if (mx > 38 && mx < 64 && my > 5 && my < 30 && bSingleStep)
        {
            Word offset = sDisplayedAsm[my - 6];
            (mapBreakpoints[offset]) ?
                mapBreakpoints[offset] = false :
                mapBreakpoints[offset] = true;
        }
    }
    last_LMB = (btns & 1);
    // right mouse button clicked
    static bool last_RMB = false;
    if (btns & 4 && !last_RMB)
    {
        // right-clicked on breakpoint
        if (mx >= 55 && my >= 33)
        {
            int index = (my - 33) + mw_brk_offset;
            // build a vector of active breakpoints
            std::vector<Word> breakpoints;
            for (auto& bp : mapBreakpoints)
                if (bp.second)
                    breakpoints.push_back(bp.first);
            if (index < breakpoints.size())
            {
                //printf("RIGHT CLICK: $%04X\n", breakpoints[index]);
                mapBreakpoints[breakpoints[index]] = false;
            }
        }

        // on PC register
        if (my == 4 && mx > 42 && mx < 47)
        {
            bSingleStep = !bSingleStep;
            if (!bSingleStep)
                nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
        }
        // right-click on code line toggles breakpoint and resumes execution
        if (mx > 38 && mx < 64 && my > 5 && my < 30 && bSingleStep)
        {
            Word offset = sDisplayedAsm[my - 6];
            (mapBreakpoints[offset]) ?
                mapBreakpoints[offset] = false :
                mapBreakpoints[offset] = true;
            if (mapBreakpoints[offset] == true)
                bSingleStep = false;
        }
    }
    last_RMB = (btns & 4);
}

void GfxDebug::KeyboardStuff()
{
    if (!bIsCursorVisible)	return;

    C6809* cpu = Bus::Inst().m_cpu;


    SDL_Keycode hx[] = { SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
                         SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
                         SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
                         SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F };
    char k[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    char d[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    static bool state[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    const Uint8* keybfr = SDL_GetKeyboardState(NULL);

    // check for valid key presses
    bool bKeyPressed = false;
    Byte ch = 0;
    for (int t = 0; t < 16; t++)
    {
        if (state[t] == 0 && keybfr[hx[t]])
        {
            state[t] = 1;
            bKeyPressed = true;
            ch = d[t];
            // printf("%c\n", k[t]);
        }
        // reset the key
        if (state[t] == 1 && !keybfr[hx[t]])	state[t] = 0;
    }

    // respond to [DEL]
    // ...

    if (!bKeyPressed)	return;

    switch (csr_at)
    {
        case CSR_AT::CSR_AT_ADDRESS:
        {
            Word addr = 0;
            if (csr_y < 10) addr = mem_bank[0] + ((csr_y - 1) * 8);
            else if (csr_y < 20) addr = mem_bank[1] + ((csr_y - 11) * 8);
            else if (csr_y < 30) addr = mem_bank[2] + ((csr_y - 21) * 8);
            Byte digit = csr_x - 1;
            if (digit == 0)	addr = (addr & 0x0fff) | (ch << 12);
            if (digit == 1)	addr = (addr & 0xf0ff) | (ch << 8);
            if (digit == 2)	addr = (addr & 0xff0f) | (ch << 4);
            if (digit == 3)	addr = (addr & 0xfff0) | (ch << 0);
            if (csr_y < 10)			mem_bank[0] = addr - ((csr_y - 1) * 8);
            else if (csr_y < 20)	mem_bank[1] = addr - ((csr_y - 11) * 8);
            else if (csr_y < 30)	mem_bank[2] = addr - ((csr_y - 21) * 8);
            if (csr_x < 5)	while (!CoordIsValid(++csr_x, csr_y));
            break;
        }
        case CSR_AT::CSR_AT_DATA:
        {
            Byte ofs = ((csr_x - 5) / 3) + ((csr_y - 1) * 8);
            Byte digit = ((csr_x + 1) % 3) - 1;
            Word addr = mem_bank[0];
            if (csr_y > 10 && csr_y < 20) { ofs -= 80; addr = mem_bank[1]; }
            if (csr_y > 20) { ofs -= 160; addr = mem_bank[2]; }
            Byte data = Bus::Read(addr + ofs,true);
            if (digit == 0)		data = (data & 0x0f) | (ch << 4);
            if (digit == 1)		data = (data & 0xf0) | (ch << 0);
            Bus::Write(addr + ofs, data);
            if (csr_x < 28)		while (!CoordIsValid(++csr_x, csr_y));
            break;
        }
    }
}










































// ********************
// * Button Callbacks *
// ********************

void GfxDebug::cbClearBreaks()
{
    if (mapBreakpoints.size() > 0)
        mapBreakpoints.clear();
}
void GfxDebug::cbReset()
{
    C6809* cpu = Bus::Inst().m_cpu;
    cpu->reset();
    mousewheel_offset = 0;
    bMouseWheelActive = false;
    bSingleStep = true;
    bIsStepPaused = true;
}
void GfxDebug::cbNMI()
{
    C6809* cpu = Bus::Inst().m_cpu;
    cpu->nmi();
    bIsStepPaused = false;
}
void GfxDebug::cbIRQ()
{
    C6809* cpu = Bus::Inst().m_cpu;
    cpu->irq();
    bIsStepPaused = false;
}
void GfxDebug::cbFIRQ()
{    
    C6809* cpu = Bus::Inst().m_cpu;
    cpu->firq();
    bIsStepPaused = false;
}
void GfxDebug::cbRunStop()
{
    (bSingleStep) ? bSingleStep = false : bSingleStep = true;
    bIsStepPaused = bSingleStep;
    nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;
}
void GfxDebug::cbHide()
{
    bMouseWheelActive = false;
    bSingleStep = false;
    bIsStepPaused = bSingleStep;
    nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    _bIsDebugActive = false;
}
void GfxDebug::cbStepIn()
{
    bSingleStep = true;
    bIsStepPaused = false;
    nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;
}
void GfxDebug::cbStepOver()
{
    bSingleStep = true;
    bIsStepPaused = false;
    nRegisterBeingEdited.reg = GfxDebug::EDIT_REGISTER::EDIT_NONE;	// cancel any register edits
    bMouseWheelActive = false;
}
void GfxDebug::cbAddBrk()
{
    printf("Add Breakpoint\n");
    bEditingBreakpoint = true;
    nRegisterBeingEdited.reg = EDIT_REGISTER::EDIT_BREAK;
}

