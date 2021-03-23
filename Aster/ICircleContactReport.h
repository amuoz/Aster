#ifndef ICIRCLE_CONTACT_REPORT_H
#define ICIRCLE_CONTACT_REPORT_H

#include "Physics.h"

class ICircleContactReport
{
public:
	virtual void OnContact(Physics::PhysicActor* other) = 0;
};


#endif
