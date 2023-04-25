// Bus.h
//
#ifndef __BUS_H__
#define __BUS_H__

class Bus
{
    private:
        Bus();

        static Bus* s_bus;
        static void _quit();

    public:    
        ~Bus();

        Bus(Bus &other) = delete;
        void operator=(const Bus &) = delete;

        static Bus* GetInstance();
        static void run();
        
};




#endif // __BUS_H__
