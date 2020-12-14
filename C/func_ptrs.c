// Original code by JB_DELR, C: https://stackoverflow.com/questions/65293973/store-function-pointer-passed-from-a-function/65294399?noredirect=1#65293973
// Function Pointer Example - assigning to a struct
// See also: https://chgi.developpez.com/c/objet/
// Interpretation/version of Button_Create/Button_CreatePtr functions by Todor, 14.12.2020

#include <stdio>
//#include <iostream>
//or whatever

typedef enum { POUSSOIR, INTERRUPTEUR } Button_Type;
typedef struct Button {
    char name[20];
    Button_Type type;
    void(*action)(Button*);
    char input[20];
    char output[20];
} Button;

static Button buttons[7];

void LOG(const char* s) {
    printf_s(s);
}

void Button_CreatePtr(Button* b, const char* name, Button_Type type, void(*action)(Button*), const char input[20], const char output[20]) {
    //Button this;
    strcpy_s(b->name, name);
    b->type = type;
    b->action = action; // (*action)(&b);
    strcpy_s(b->input, input);
    strcpy_s(b->output, output);
}

    void Button_Create(Button &b, const char* name, Button_Type type, void(*action)(Button*), const char input[20], const char output[20]) {
        //Button this;
        strcpy_s(b.name, name);
        b.type = type;
        b.action = action; // (*action)(&b);
        strcpy_s(b.input, input);
        strcpy_s(b.output, output);        
    }

    void dimmer(Button* button) {
        LOG("dim!!!");
    }

    int init_Buttons() {
        //buttons[0] = Button_Create("Salon", POUSSOIR, &dimmer, "DI04", "DMX_2");
        Button_Create(buttons[0], "Salon", POUSSOIR, &dimmer, "DI04", "DMX_2");
        Button_CreatePtr(&buttons[1], "Baloon", INTERRUPTEUR, &dimmer, "GYZ1", "BTS2");
        //Button_CreatePtr
        printf_s(buttons[0].input);
        printf_s("\n");
        printf_s(buttons[0].output);
        printf_s(buttons[1].input);
        printf_s("\n");
        printf_s(buttons[1].output);
        return 1;
    }
		
int main()
{
    init_Buttons();
	  return 1;
}
