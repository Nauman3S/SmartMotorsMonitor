void setupButtons()
{
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT_PULLUP);

    pinMode(J1_BUTTON_PIN, INPUT_PULLUP);
    pinMode(J2_BUTTON_PIN, INPUT_PULLUP);
}

uint8_t getButtonState(int btn)
{
    if (btn == BTN_1)
    {
        digitalRead(BTN1_PIN);
    }
    else if (btn == BTN_2)
    {
        digitalRead(BTN2_PIN);
    }
    else if (btn == J1_BTN)
    {
        digitalRead(J1_BUTTON_PIN);
    }
    else if (btn == J2_BTN)
    {
        digitalRead(J2_BUTTON_PIN);
    }
}