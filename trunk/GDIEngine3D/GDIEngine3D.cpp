// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"
#include "GDIEngine3D.h"

#include "thread_safety.h"
#include "Render.h"

// ============================================================================
// CMutualAccessor partial implementation

template <class T>
class CMutualAccessor : public IMutualAccessible
{
public:
	CMutualAccessor() : IMutualAccessible(new T()) { };
	~CMutualAccessor() 
	{ 
		delete (T*)IMutualAccessible::Lock(); 
	};

	inline T& Lock() 
	{ 
		return *(T*)IMutualAccessible::Lock(); 
	};
};

#define _NEW_LPOBJECT_VAR(type_name, var_name) \
	CMutualAccessor<type_name> *var_name = new CMutualAccessor<type_name>()

#define _RETURN_HANDLE(handle_type_name, var_name) \
	return (handle_type_name)var_name

#define _IS_HANDLE_VALID(type_name, handle_name)											\
	handle_name != NULL																		\
	&& thread_safety::AccessorExists((LPMUTUAL_ACCESSIBLE)handle_name)						\
	&& typeid(*((LPMUTUAL_ACCESSIBLE)handle_name)) == typeid(CMutualAccessor<type_name>)

#define _RETURN_REMOVE_RESULT(handle_name) \
	return thread_safety::RemoveAccessor((LPMUTUAL_ACCESSIBLE)handle_name)

#define _HANDLE_CAST(type_name, handle_name) \
	((CMutualAccessor<type_name>*)handle_name)

void accessor_cleanup(LPMUTUAL_ACCESSIBLE lpAcObj) { delete lpAcObj; }

// ============================================================================
// CRenderPool library interface implementation

HRENDERPOOL3D WINAPI CreateRenderPool3D()
{
	_NEW_LPOBJECT_VAR(RENDER_POOL, lpRpAcsr);
	thread_safety::AddAccessor(lpRpAcsr);
	_RETURN_HANDLE(HRENDERPOOL3D, lpRpAcsr);
}

BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D)
{
	if (!_IS_HANDLE_VALID(RENDER_POOL, hRp3D)) return FALSE;
	_RETURN_REMOVE_RESULT(hRp3D);
}

BOOL WINAPI RenderPoolAssignScene3D(HRENDERPOOL3D hRp3D, HSCENE3D hScn3D)
{
	if (!(_IS_HANDLE_VALID(RENDER_POOL, hRp3D) 
		&& _IS_HANDLE_VALID(SCENE3D, hScn3D))) return FALSE;

	return _HANDLE_CAST(RENDER_POOL, hRp3D)->Lock().assignScene(
		&_HANDLE_CAST(SCENE3D, hScn3D)->Lock()
		);
}

// TODO: Implement the most useful interface functions, such as: CreateLight, CreateCamera, RenderWorld(CRenderPool based) and so on 

