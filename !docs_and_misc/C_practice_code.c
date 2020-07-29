typedef struct {
    int value;
    int enabled;
} Counter; // Class counter = template for creating an object

typedef struct {
    Counter super; // Full superclass object not just pointer
    int nResets;
} ResetCounter; // Class ResetCounter inheriting Counter + one own variable

#define initCounter(en) {0,en} // Preprocessor macro initX to define initialization of an object
#define initResetCounter(en) { initCounter(en), 0 }; // -.- Inheriting class

Counter cnt = initCounter(1); // Creating the object cnt
ResetCounter rcnt = initResetCounter(1);// Creating the object Rcnt that is a counter object. Can now use Counter's methods (aslong as we typecast).

int inc(Counter *self,int arg) { //*self = object pointed to by self
    if (self->enabled) {
        self->value = self->value + arg;
    } return self->value;
}

int enable(Counter *self, int arg) {
    self->enabled = arg;
    return 0;
}
int reset ( ResetCounter *self, int arg) { //Unique method to ResetCounter class
    self->super.value = 0; //instance variable of superclass object
    self->nResets++;
    return 0;
}

void main(void) {
    int print = inc( &cnt,10 ); // Invoking inc with current object, &cnt = address of cnt
    printf("%d\n",print);


    int print2 = inc( (Counter*)&rcnt,4); // Typecasting with (Counter*)
    printf("%d\n",print2);
}
 