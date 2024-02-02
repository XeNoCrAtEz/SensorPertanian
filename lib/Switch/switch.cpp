#include "switch.h"


Switch::Switch(uint8_t pin) 
        : m_pin(pin) {
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);

    m_status = READY;
}


Switch::OpStatus Switch::on() {
    if (m_status != READY) return STATUS_ERROR;
    
    if (m_level == HIGH) return ON;
    digitalWrite(m_pin, HIGH);

    return ON;
}


Switch::OpStatus Switch::off() {
    if (m_status != READY) return STATUS_ERROR;
    
    if (m_level == LOW) return OFF;
    digitalWrite(m_pin, LOW);

    return OFF;
}


Switch::Status Switch::release() {
    pinMode(m_pin, INPUT);
    m_status = RELEASED;
    return RELEASED;
}


Switch::Status Switch::status() {
    return m_status;
}