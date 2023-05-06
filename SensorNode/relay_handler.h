void setupRelays()
{
    pinMode(RELAY_PIN1, OUTPUT);
    pinMode(RELAY_PIN2, OUTPUT);
    pinMode(RELAY_PIN3, OUTPUT);
    pinMode(RELAY_PIN4, OUTPUT);
    
}

void setRelay(int relay, int state)
{
    if (relay == RELAY1)
    {
        digitalWrite(RELAY_PIN1, state);
        digitalWrite(~RELAY_PIN2, state);
    }
    else if (relay == RELAY2)
    {
        
    }
    else if (relay == RELAY3)
    {
        digitalWrite(RELAY_PIN3, state);
        digitalWrite(~RELAY_PIN4, state);
    }
    else if (relay == RELAY4)
    {
        
    }
}