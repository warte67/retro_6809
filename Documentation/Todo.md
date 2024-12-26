# To Do Notes:

## IDevice
- The initial RAM device has been implemented.
- We still need to implement a std::map of all of the memory based devices rather than hard code as memory_map.hpp.
  ```cpp
    struct _memory_node {
        Word start_addr;                    // identifies the starting address of the device
        std::string name;                   // name of the device
        std::string description;            // description of the device
        std::vector<std::string> comments;  // comments about the device
    }
  ```