#ifdef INTERFACE
CLASS(NexuizCheckBoxString) EXTENDS(CheckBox)
	METHOD(NexuizCheckBoxString, configureNexuizCheckBoxString, void(entity, string, string, string, string))
	METHOD(NexuizCheckBoxString, setChecked, void(entity, float))
	ATTRIB(NexuizCheckBoxString, fontSize, float, SKINFONTSIZE_NORMAL)
	ATTRIB(NexuizCheckBoxString, image, string, SKINGFX_CHECKBOX)
	ATTRIB(NexuizCheckBoxString, yesString, string, string_null)
	ATTRIB(NexuizCheckBoxString, noString, string, string_null)

	ATTRIB(NexuizCheckBoxString, color, vector, SKINCOLOR_CHECKBOX_N)
	ATTRIB(NexuizCheckBoxString, colorC, vector, SKINCOLOR_CHECKBOX_C)
	ATTRIB(NexuizCheckBoxString, colorF, vector, SKINCOLOR_CHECKBOX_F)
	ATTRIB(NexuizCheckBoxString, colorD, vector, SKINCOLOR_CHECKBOX_D)

	ATTRIB(NexuizCheckBoxString, cvarName, string, string_null)
	METHOD(NexuizCheckBoxString, loadCvars, void(entity))
	METHOD(NexuizCheckBoxString, saveCvars, void(entity))

	ATTRIB(NexuizCheckBoxString, alpha, float, SKINALPHA_TEXT)
	ATTRIB(NexuizCheckBoxString, disabledAlpha, float, SKINALPHA_DISABLED)
ENDCLASS(NexuizCheckBoxString)
entity makeNexuizCheckBoxString(string, string, string, string);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizCheckBoxString(string theYesValue, string theNoValue, string theCvar, string theText)
{
	entity me;
	me = spawnNexuizCheckBoxString();
	me.configureNexuizCheckBoxString(me, theYesValue, theNoValue, theCvar, theText);
	return me;
}
void NexuizCheckBoxString_configureNexuizCheckBoxString(entity me, string theYesValue, string theNoValue, string theCvar, string theText)
{
	me.yesString = theYesValue;
	me.noString = theNoValue;
	me.checked = 0;
	if(theCvar)
	{
		me.cvarName = theCvar;
		me.tooltip = getZonedTooltipForIdentifier(theCvar);
		me.loadCvars(me);
	}
	me.configureCheckBox(me, theText, me.fontSize, me.image);
}
void NexuizCheckBoxString_setChecked(entity me, float foo)
{
	me.checked = !me.checked;
	me.saveCvars(me);
}
void NexuizCheckBoxString_loadCvars(entity me)
{
	if not(me.cvarName)
		return;

	if(cvar_string(me.cvarName) == me.yesString)
		me.checked = 1;
}
void NexuizCheckBoxString_saveCvars(entity me)
{
	if not(me.cvarName)
		return;

	if(me.checked)
		cvar_set(me.cvarName, me.yesString);
	else
		cvar_set(me.cvarName, me.noString);
}
#endif
