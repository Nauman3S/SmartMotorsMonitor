void setupDigitalInputs()
{
    pinMode(D_IN1_PIN, INPUT_PULLUP);
    pinMode(D_IN2_PIN, INPUT_PULLUP);
    
}

uint8_t getDigitalInputState(int btn)
{
    if (btn == D_IN1)
    {
        digitalRead(D_IN1_PIN);
    }
    else if (btn == D_IN2)
    {
        digitalRead(D_IN2_PIN);
    }
}