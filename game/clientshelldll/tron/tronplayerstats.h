// ----------------------------------------------------------------------- //
//
// MODULE  : TO2PlayerStats.h
//
// PURPOSE : Definition of PlayerStats class
//
// CREATED : 10/9/97
//
// (c) 1997-2000 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __TRONPLAYERSTATS_H
#define __TRONPLAYERSTATS_H

#include "ltbasedefs.h"
#include "weaponmgr.h"
#include "overlays.h"
#include "idlist.h"
#include "intelitemlist.h"
#include "skillsbutemgr.h"
#include "playerstats.h"
#include "ratingmgr.h"
#include "sharedmission.h"

class CTronPlayerStats : public CPlayerStats
{
public:

	CTronPlayerStats();
	~CTronPlayerStats();

	IDList*		GetObjectives()				{ return &m_Objectives; }
	IDList*		GetOptionalObjectives()		{ return &m_OptionalObjectives; }
	IDList*		GetCompletedObjectives()	{ return &m_CompletedObjectives; }
	IDList*		GetParameters()				{ return &m_Parameters; }
	IDList*		GetKeys()					{ return &m_Keys; }
	
	CIntelItemList* GetIntelList()			{ return &m_IntelList; }

    LTBOOL      Init();
	void		Term();

    void        OnEnterWorld(LTBOOL bRestoringGame=LTFALSE);
	void		OnExitWorld();

    void        UpdatePlayerWeapon(uint8 nWeaponId, uint8 nAmmoId, LTBOOL bForce=LTFALSE);
	void		ResetStats();

	void		Clear();
    void        UpdateHealth(uint32 nHealth);
    void        UpdateArmor(uint32 nArmor);
    void        UpdateMaxHealth(uint32 nHealth);
    void        UpdateMaxArmor(uint32 nArmor);
    void        UpdateAmmo(uint8 nWeaponId, uint8 nAmmoId, uint32 nAmmo, LTBOOL bPickedup=LTFALSE, LTBOOL bDisplayMsg=LTTRUE);
    void        UpdateGear(uint8 nGearId);
    void        UpdateMod(uint8 nModId);
    void        UpdateAir(LTFLOAT nPercent);
    void        UpdateObjectives(uint8 nThing, uint8 nType, uint32 dwId);
    void        UpdateKeys(uint8 nType, uint16 dwId);
	void        UpdateIntel(uint32 nTextId, uint8 nPopupId, LTBOOL bIsIntel);
    void        UpdateHiding(LTBOOL bIsHiding,LTBOOL bIsHidden);
	void		UpdateSkills(ILTMessage_Read *pMsg);
	void		GainSkills(uint8 nRewardId);
	void		UpdateMissionStats(ILTMessage_Read *pMsg);
	void		UpdateProgress(uint32 nProgress) {}
	void		UpdateMaxProgress(uint32 nMaxProgress) {}

	void		ResetSkills();						 //clears all skill data
	void		ResetMissionStats();

	void		ResetInventory();						 //clears all inventory data
	void		DropInventory(LTBOOL bResetGear=LTTRUE); //drops are currently carried weapons and ammo
														 // also drops gear and mods if bResetGear is set

	void		SetMultiplayerObjectives(ILTMessage_Read *pMsg);

	void		ClearMissionDamage()	{m_nDamage = 0;}
	int			GetMissionDamage()		{return m_nDamage;}

	const MissionStats* GetMissionStats() const {return &m_MissionStats;}


	void		Save(ILTMessage_Write *pMsg);
	void		Load(ILTMessage_Read *pMsg);

    uint8       GetCurWeapon() const { return m_nCurrentWeapon; }

    uint8           GetMod(ModType eType, const WEAPON* pW=LTNULL);

    uint32       GetAmmoCount(uint8 nAmmoId) const;
    LTBOOL       HaveWeapon(uint8 nWeaponId) const;
    LTBOOL       HaveMod(uint8 nModId) const;
    LTBOOL       HaveGear(uint8 nGearId) const;

    LTBOOL       CanUseWeapon(uint8 nWeaponId) const;
    LTBOOL       CanUseAmmo(uint8 nAmmoId) const;
    LTBOOL       CanUseMod(uint8 nModId) const;
    LTBOOL       CanUseGear(uint8 nGearId) const;


	void		Setup( );

    LTBOOL      HaveAirSupply();


    uint32      GetHealth()		{ return m_nHealth;}	// current health
    uint32      GetMaxHealth()	{ return m_nMaxHealth;}	// current maximum health
    uint32      GetArmor()		{ return m_nArmor;}		// current armor
    uint32      GetMaxArmor()	{ return m_nMaxArmor;}	// current maximum armor
    LTFLOAT		GetAirPercent()	{ return m_fAirPercent;}

	uint32		GetProgress() { return 0; }
	uint32		GetMaxProgress() { return 0; }

    uint8       GetCurrentWeapon()	{ return m_nCurrentWeapon;}	// current weapon
    uint8       GetCurrentAmmo()	{ return m_nCurrentAmmo;}	// current ammo

	uint32		GetCurrentAmmoCount();

	//////////////////////////////
	///// TEMP!!!!!!!!!!!!!!!!!!
	// Why are these here???
	//////////////////////////////
	virtual inline uint8    GetSilencer(const WEAPON* pW=LTNULL) { return 0; }
    virtual inline uint8    GetScope(const WEAPON* pW=LTNULL) { return 0; }
    virtual inline LTBOOL    CanBeSilenced() { return LTFALSE; }
    virtual inline LTBOOL    CanHaveScope() { return LTFALSE; }

	LTBOOL		IsHiding()	{return m_bHiding;}
	LTBOOL		IsHidden()	{return m_bHidden;}

	uint32		GetTotalSkillPoints() {return m_nTotalSkillPoints;}
	uint32		GetAvailSkillPoints() {return m_nAvailSkillPoints;}
	uint8		GetSkillLevel(eSkill skill) {return m_nSkills[skill]; }

	const RANK*	GetRank();
	uint32		GetCostToUpgrade(eSkill skill);
	const StealthModifiers* GetStealthModifiers() 
		{return g_pSkillsButeMgr->GetStealth((eSkillLevel)m_nSkills[SKL_STEALTH]);}

	const StaminaModifiers* GetStaminaModifiers() 
		{return g_pSkillsButeMgr->GetStamina((eSkillLevel)m_nSkills[SKL_STAMINA]);}

	const AimModifiers* GetAimModifiers() 
		{return g_pSkillsButeMgr->GetAim((eSkillLevel)m_nSkills[SKL_AIM]);}

	const CarryModifiers* GetCarryModifiers() 
		{return g_pSkillsButeMgr->GetCarry((eSkillLevel)m_nSkills[SKL_CARRY]);}

	const ArmorModifiers* GetArmorModifiers() 
		{return g_pSkillsButeMgr->GetArmor((eSkillLevel)m_nSkills[SKL_ARMOR]);}

	const WeaponModifiers* GetWeaponModifiers() 
		{return g_pSkillsButeMgr->GetWeapon((eSkillLevel)m_nSkills[SKL_WEAPON]);}

	const GadgetModifiers* GetGadgetModifiers() 
		{return g_pSkillsButeMgr->GetGadget((eSkillLevel)m_nSkills[SKL_GADGET]);}

	const EnergyModifiers* GetEnergyModifiers()
		{return g_pSkillsButeMgr->GetEnergy((eSkillLevel)m_nSkills[SKL_ENERGY]);}

	const SearchModifiers* GetSearchModifiers() 
		{return g_pSkillsButeMgr->GetSearch((eSkillLevel)m_nSkills[SKL_STEALTH]);}

	// ABM Tron stuff
	uint32 GetEnergy();//			{ return m_nEnergy;}	// Energy levels
	uint32 GetMaxEnergy()		{ return m_nMaxEnergy;}
	void UpdateEnergy(uint32 nEnergy);
	void UpdateMaxEnergy(uint32 nEnergy);
	
	void	ClearPermissions();
	void	ClearRequiredPermissions();

	void	SetPermissions(uint8 nPermissionSet);
	void	SetRequiredPermissions(uint8 nPermissionSet);

	bool	HasRightPermissions();
	bool	HasPermission(uint8 nPermission);
	bool	HasPermissionSet(uint8 nPermissionSet);
	void	GetPermissions(uint8 &iPermissions, uint8 &iRequiredPermissions);

	// Version
	void SetJetVersion(uint16 iVersion);
	void GetJetVersion(uint16 &iVersion);
	void IncrementJetVersion(uint8 iVersionPoints);

	// Performance Ratings
	int8	GetRating(PerformanceRating p) {return m_RatingMgr.GetRating(p);}
	void	SetPerformanceRating(PerformanceRating p, int i) {m_RatingMgr.SetPerformanceRating(p,i);}
	void	GivePlayerAdditive(char * pName) {m_RatingMgr.GivePlayerAdditive(pName);}

	// Subroutine Armor
    void    UpdateSubroutineArmor(float fArmor, float fVirus, float fScalar);

	// Compile
	void	Compile();	// called when the player compiles()

protected:

    void        AddCanUseWeapon(uint8 nWeaponId);
    void        AddCanUseAmmo(uint8 nAmmoId);
    void        AddCanUseMod(uint8 nModId);
    void        AddCanUseGear(uint8 nGearId);


protected:

	uint32		m_nHealth;			// current health
	uint32		m_nArmor;			// current armor
	uint32		m_nMaxHealth;		// current maximum health
	uint32		m_nMaxArmor;		// current maximum armor
	LTFLOAT		m_fAirPercent;
	uint32*		m_pnAmmo;			// All ammo values
	LTBOOL*		m_pbHaveAmmo;		// ammos that player had during mission
	LTBOOL*		m_pbHaveWeapon;		// Weapon status
	uint8		m_nCurrentWeapon;	// current weapon
	uint8		m_nCurrentAmmo;		// current ammo
	LTBOOL*		m_pbHaveMod;		// Mod status
	LTBOOL*		m_pbHaveGear;		// Gear status

	uint32		m_nTotalSkillPoints;
	uint32		m_nAvailSkillPoints;
	uint8		m_nSkills[kNumSkills];

	LTBOOL*		m_pbCanUseAmmo;		// Can we use this ammo
	LTBOOL*		m_pbCanUseWeapon;	// Can we carry this Weapon
	LTBOOL*		m_pbCanUseMod;		// Can we carry this Mod
	LTBOOL*		m_pbCanUseGear;		// Can we carry this Gear

	int			m_nDamage;			// Damage taken during this mission

	LTBOOL		m_bHidden;			//player is hidden
	LTBOOL		m_bHiding;			//player is trying to hide

	IDList	m_Objectives;
	IDList	m_OptionalObjectives;
	IDList	m_CompletedObjectives;
	IDList	m_Parameters;
	IDList	m_Keys;
	
	CIntelItemList	m_IntelList;


	LTBOOL			m_bObjAdded;
	LTBOOL			m_bObjRemoved;
	LTBOOL			m_bObjCompleted;

	// ABM new stuff
	uint16			m_nVersion;			// Jet's version number (major, minor, sub)

	uint32			m_nEnergy;			// energy for shields, ammo, subroutines
	uint32			m_nMaxEnergy;

	uint8			m_nPermissions;		// 8-bits of which permissions the player has
	uint8			m_nRequiredPermissions;	// 8 bits of which permissions the player needs

	float			m_fSubroutineArmor;
	float			m_fVirusArmor;
	float			m_fArmorScalar;

	// Performance ratings
	CRatingMgr		m_RatingMgr;

	MissionStats	m_MissionStats;
};

extern CTronPlayerStats* g_pTronPlayerStats;

#endif // __TRONPLAYERSTATS_H
