#define __WINESRC__

#include "../common/common.h"

/*
	NP Class
	These function *should* never be called from a plugin.
	The plugin has to use the browser API instead, so we just
	need stubs to detect a violation of the API.
*/

void NPInvalidateFunction(NPObject *npobj){
	DBG_TRACE("( npobj=%p )", npobj);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> void");
}

bool NPHasMethodFunction(NPObject *npobj, NPIdentifier name){
	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPInvokeFunction(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result){
	DBG_TRACE("( npobj=%p, name=%p, args=%p, argCount=%d, result=%p )", npobj, name, args, argCount, result);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPInvokeDefaultFunction(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result){
	DBG_TRACE("( npobj=%p, args=%p, argCount=%d, result=%p )", npobj, args, argCount, result);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPHasPropertyFunction(NPObject *npobj, NPIdentifier name){
	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPGetPropertyFunction(NPObject *npobj, NPIdentifier name, NPVariant *result){
	DBG_TRACE("( npobj=%p, name=%p, result=%p )", npobj, name, result);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPSetPropertyFunction(NPObject *npobj, NPIdentifier name, const NPVariant *value){
	DBG_TRACE("( npobj=%p, name=%p, result=%p )", npobj, name, value);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPRemovePropertyFunction(NPObject *npobj, NPIdentifier name){
	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPEnumerationFunction(NPObject *npobj, NPIdentifier **value, uint32_t *count){
	DBG_TRACE("( npobj=%p, value=%p, count=%p )", npobj, value, count);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

bool NPConstructFunction(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result){
	DBG_TRACE("( npobj=%p, args=%p, argCount=%d, result=%p )", npobj, args, argCount, result);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

NPObject * NPAllocateFunction(NPP instance, NPClass *aClass){
	DBG_TRACE("( instance=%p, aClass=%p )", instance, aClass);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> obj=NULL");
	return NULL;
}

void NPDeallocateFunction(NPObject *npobj){
	DBG_TRACE("( npobj=%p )", npobj);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> void");
}

NPClass myClass = {
	NP_CLASS_STRUCT_VERSION,
	NULL, // NPAllocateFunction,
	NULL, // NPDeallocateFunction,
	NPInvalidateFunction,
	NPHasMethodFunction,
	NPInvokeFunction,
	NPInvokeDefaultFunction, 
	NPHasPropertyFunction,
	NPGetPropertyFunction,
	NPSetPropertyFunction,
	NPRemovePropertyFunction,
	NPEnumerationFunction,
	NPConstructFunction	
};
