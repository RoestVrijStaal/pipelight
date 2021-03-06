/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is fds-team.de code.
 *
 * The Initial Developer of the Original Code is
 * Michael Müller <michael@fds-team.de>
 * Portions created by the Initial Developer are Copyright (C) 2013
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Michael Müller <michael@fds-team.de>
 *   Sebastian Lackner <sebastian@fds-team.de>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <string.h>								// for memcpy

#include "common/common.h"

struct ProxyObject
{
	NPObject obj;
	Context *ctx;
};

static void NPInvalidateFunction(NPObject *npobj)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p )", npobj);

	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_INVALIDATE);
	ctx->readResultVoid();

	DBG_TRACE(" -> void");
}

static bool NPHasMethodFunction(NPObject *npobj, NPIdentifier name)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);

	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_HAS_METHOD);

	bool resultBool = (bool)ctx->readResultInt32();
	DBG_TRACE(" -> result=%d", resultBool);
	return resultBool;
}

static bool NPInvokeFunction(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;
	Stack stack;

	DBG_TRACE("( npobj=%p, name=%p, args[]=%p, argCount=%d, result=%p )", npobj, name, args, argCount, result);

	/* warning: parameter order swapped! */
	ctx->writeVariantArrayConst(args, argCount);
	ctx->writeInt32(argCount);
	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_INVOKE);
	ctx->readCommands(stack);

	bool resultBool = (bool)readInt32(stack);

	if (resultBool)
		readVariant(stack, *result); /* refcount already incremented by invoke() */
	else
	{
		result->type				= NPVariantType_Void;
		result->value.objectValue	= NULL;
	}

	DBG_TRACE(" -> ( result=%d, ... )", resultBool);
	return resultBool;
}

static bool NPInvokeDefaultFunction(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;
	Stack stack;

	DBG_TRACE("( npobj=%p, args=%p, argCount=%d, result=%p )", npobj, args, argCount, result);

	ctx->writeVariantArrayConst(args, argCount);
	ctx->writeInt32(argCount);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_INVOKE_DEFAULT);
	ctx->readCommands(stack);

	bool resultBool = (bool)readInt32(stack);

	if (resultBool)
		readVariant(stack, *result); /* refcount already incremented by invoke() */
	else
	{
		result->type				= NPVariantType_Void;
		result->value.objectValue	= NULL;
	}

	DBG_TRACE(" -> ( result=%d, ... )", resultBool);
	return resultBool;
}

static bool NPHasPropertyFunction(NPObject *npobj, NPIdentifier name)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);

	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_HAS_PROPERTY);

	bool resultBool = (bool)ctx->readResultInt32();
	DBG_TRACE(" -> result=%d", resultBool);
	return resultBool;
}

static bool NPGetPropertyFunction(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;
	Stack stack;

	DBG_TRACE("( npobj=%p, name=%p, result=%p )", npobj, name, result);

	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_GET_PROPERTY);
	ctx->readCommands(stack);

	bool resultBool = readInt32(stack); /* refcount already incremented by getProperty() */

	if (resultBool)
		readVariant(stack, *result);
	else
	{
		result->type				= NPVariantType_Void;
		result->value.objectValue	= NULL;
	}

	DBG_TRACE(" -> ( result=%d, ... )", resultBool);
	return resultBool;
}

static bool NPSetPropertyFunction(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p, name=%p, value=%p )", npobj, name, value);

	ctx->writeVariantConst(*value);
	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_SET_PROPERTY);

	bool resultBool = (bool)ctx->readResultInt32();
	DBG_TRACE(" -> result=%d", resultBool);
	return resultBool;
}

static bool NPRemovePropertyFunction(NPObject *npobj, NPIdentifier name)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p, name=%p )", npobj, name);

	ctx->writeHandleIdentifier(name);
	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_REMOVE_PROPERTY);

	bool resultBool = (bool)ctx->readResultInt32();
	DBG_TRACE(" -> result=%d", resultBool);
	return resultBool;
}

static bool NPEnumerationFunction(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;
	Stack stack;

	DBG_TRACE("( npobj=%p, value=%p, count=%p )", npobj, value, count);

	ctx->writeHandleObj(npobj);
	ctx->callFunction(FUNCTION_NP_ENUMERATE);
	ctx->readCommands(stack);

	bool result = (bool)readInt32(stack);
	if (result){

		uint32_t identifierCount = readInt32(stack);
		if (identifierCount == 0){
			*value = NULL;
			*count = 0;

		}else{
			std::vector<NPIdentifier> identifiers	= readIdentifierArray(stack, identifierCount);
			NPIdentifier* identifierTable			= (NPIdentifier*)sBrowserFuncs->memalloc(identifierCount * sizeof(NPIdentifier));

			if (identifierTable){
				memcpy(identifierTable, identifiers.data(), sizeof(NPIdentifier) * identifierCount);

				*value = identifierTable;
				*count = identifierCount;

			}else{
				result = false;

			}
		}
	}

	DBG_TRACE(" -> ( result=%d, ... )", result);
	return result;
}

static bool NPConstructFunction(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	DBG_TRACE("( npobj=%p, args=%p, argCount=%d, result=%p )", npobj, args, argCount, result);
	NOTIMPLEMENTED();
	DBG_TRACE(" -> result=0");
	return false;
}

static NPObject *NPAllocateFunction(NPP instance, NPClass *aClass)
{
	struct ProxyObject *proxy;

	DBG_TRACE("( instance=%p, aClass=%p )", instance, aClass);

	proxy = (ProxyObject *)malloc(sizeof(struct ProxyObject));
	if (proxy)
	{
		proxy->obj._class = aClass;
		proxy->obj.referenceCount = 1;
		proxy->ctx = ctx; /* FIXME: Obtain from instance->pdata */
	}

	DBG_TRACE(" -> npobj=%p", &proxy->obj);
	return &proxy->obj;
}

static void NPDeallocateFunction(NPObject *npobj)
{
	struct ProxyObject *proxy = (struct ProxyObject *)npobj;
	Context *ctx = proxy->ctx;

	DBG_TRACE("( npobj=%p )", npobj);

	if (handleManager_existsByPtr(HMGR_TYPE_NPObject, npobj))
	{
		DBG_TRACE("seems to be a user created handle, calling WIN_HANDLE_MANAGER_FREE_OBJECT( obj=%p ).", npobj);

		/* kill the object on the other side */
		ctx->writeHandleObj(npobj);
		ctx->callFunction(WIN_HANDLE_MANAGER_FREE_OBJECT);
		ctx->readResultVoid();

		/* remove it in the handle manager */
		handleManager_removeByPtr(HMGR_TYPE_NPObject, npobj);
	}

	/* remove the object locally */
	free(proxy);
	DBG_TRACE(" -> void");
}

NPClass proxy_class = {
	NP_CLASS_STRUCT_VERSION,
	NPAllocateFunction,
	NPDeallocateFunction,
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
