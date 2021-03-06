// ----------------------------------------------------------------------- //
//
// MODULE  : Key.h
//
// PURPOSE : Key definition for Keyframer class
//
// CREATED : 10/7/97
//
// ----------------------------------------------------------------------- //

#ifndef __KEY_H__
#define __KEY_H__

#include "ltengineobjects.h"
#include "commandmgr.h"

LINKTO_MODULE( Key );

class Key : public BaseClass
{
	public :
};

class CKeyPlugin : public IObjectPlugin 
{
	public :
		
		virtual LTRESULT PreHook_PropChanged(
				const char *szObjName,
				const char *szPropName,
				const int nPropType,
				const GenericProp &gpPropValue,
				ILTPreInterface *pInterface,
				const char *szModifiers );

	protected :

		CCommandMgrPlugin m_CommandMgrPlugin;

};

#endif // __KEY_H__