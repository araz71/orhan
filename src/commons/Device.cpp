#include <string>

struct Device {
    std::string m_serial;
    std::string m_label;
    std::string m_owner;

    Device(std::string serial, std::string label)
    : m_serial(serial),
    m_label(label)
    {
    }

    void set_owner(std::string owner) {
        m_owner = owner;
    }

    std::string get_owner() {
        return m_owner;
    }

    std::string get_serial() {
        return m_serial;
    }

    std::string get_label() {
        return m_label;
    }
}