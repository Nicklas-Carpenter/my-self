#ifndef SELF_OBJECT_H
#define SELF_OBJECT_H

typedef struct {
    char *thing;
} SelfObject;

SelfObject *self_object_create(char *thing);
void self_object_destroy(SelfObject *obj);

#endif // SELF_OBJECT_H
