#ifdef INTERFACE
CLASS(NexuizPlayerModelSelector) EXTENDS(NexuizImage)
	METHOD(NexuizPlayerModelSelector, configureNexuizPlayerModelSelector, void(entity))
	METHOD(NexuizPlayerModelSelector, loadCvars, void(entity))
	METHOD(NexuizPlayerModelSelector, saveCvars, void(entity))
	METHOD(NexuizPlayerModelSelector, draw, void(entity))
	METHOD(NexuizPlayerModelSelector, resizeNotify, void(entity, vector, vector, vector, vector))
	ATTRIB(NexuizPlayerModelSelector, currentModel, string, string_null)
	ATTRIB(NexuizPlayerModelSelector, currentSkin, float, 0)
	ATTRIB(NexuizPlayerModelSelector, currentModelImage, string, string_null)
	ATTRIB(NexuizPlayerModelSelector, currentModelTitle, string, string_null)
	ATTRIB(NexuizPlayerModelSelector, currentModelDescription, string, string_null)
	METHOD(NexuizPlayerModelSelector, go, void(entity, float))
	METHOD(NexuizPlayerModelSelector, destroy, void(entity))
	ATTRIB(NexuizPlayerModelSelector, origin, vector, '0 0 0')
	ATTRIB(NexuizPlayerModelSelector, size, vector, '0 0 0')
	ATTRIB(NexuizPlayerModelSelector, realFontSize, vector, '0 0 0')
	ATTRIB(NexuizPlayerModelSelector, fontSize, float, SKINFONTSIZE_NORMAL)
	ATTRIB(NexuizPlayerModelSelector, titleFontSize, float, SKINFONTSIZE_TITLE)
	ATTRIB(NexuizPlayerModelSelector, bufModels, float, -1)
	ATTRIB(NexuizPlayerModelSelector, numModels, float, -1)
	ATTRIB(NexuizPlayerModelSelector, idxModels, float, -1)
ENDCLASS(NexuizPlayerModelSelector)
entity makeNexuizPlayerModelSelector();
void PlayerModelSelector_Next_Click(entity btn, entity me);
void PlayerModelSelector_Prev_Click(entity btn, entity me);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizPlayerModelSelector()
{
	entity me;
	me = spawnNexuizPlayerModelSelector();
	me.configureNexuizPlayerModelSelector(me);
	return me;
}

#define BUFMODELS_TITLE 0
#define BUFMODELS_IMAGE 1
#define BUFMODELS_MODEL 2
#define BUFMODELS_SKIN 3
#define BUFMODELS_DESC 4
#define BUFMODELS_COUNT 5

void NexuizPlayerModelSelector_configureNexuizPlayerModelSelector(entity me)
{
	float sortbuf, glob, i;
	string fn;

	glob = search_begin(get_model_datafilename(string_null, -1, "txt"), TRUE, TRUE);
	if (glob < 0)
		return;

	me.configureNexuizImage(me, string_null, -1);

	sortbuf = buf_create();
	for(i = 0; i < search_getsize(glob); ++i)
	{
		// select model #i!
		fn = search_getfilename(glob, i);
		if(!get_model_parameters(fn, -1))
			continue;
		bufstr_add(sortbuf, sprintf("%-128s%s", get_model_parameters_name, fn), 1);
	}
	search_end(glob);
	buf_sort(sortbuf, 128, 0);
	me.numModels = buf_getsize(sortbuf);
	me.bufModels = buf_create();
	for(i = 0; i < me.numModels; ++i)
	{
		fn = substring(bufstr_get(sortbuf, i), 128, -1);
		if(!get_model_parameters(fn, -1))
			error("But it JUST worked!");
		bufstr_set(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_TITLE, get_model_parameters_name);
		bufstr_set(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_IMAGE, strcat("/", substring(get_model_datafilename(get_model_parameters_modelname, get_model_parameters_modelskin, "tga"), 0, -5)));
		bufstr_set(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_MODEL, get_model_parameters_modelname);
		bufstr_set(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_SKIN, ftos(get_model_parameters_modelskin));
		get_model_parameters_desc = strcat(get_model_parameters_desc, "\n");
		if(get_model_parameters_sex)
			get_model_parameters_desc = strcat(get_model_parameters_desc, "\nSex: ", get_model_parameters_sex);
		if(get_model_parameters_weight)
			get_model_parameters_desc = strcat(get_model_parameters_desc, "\nWeight: ", ftos(get_model_parameters_weight), " kg");
		if(get_model_parameters_age)
			get_model_parameters_desc = strcat(get_model_parameters_desc, "\nAge: ", ftos(get_model_parameters_age));
		while(substring(get_model_parameters_desc, -1, 1) == "\n")
			get_model_parameters_desc = substring(get_model_parameters_desc, 0, -2);
		bufstr_set(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_DESC, get_model_parameters_desc);
	}
	buf_del(sortbuf);
	get_model_parameters(string_null, 0);
	me.loadCvars(me); // this will select the initial model, depending on the current cvars
	me.go(me, 0); // this will set the vars for the selected model
}
void NexuizPlayerModelSelector_destroy(entity me)
{
	buf_del(me.bufModels);
	me.bufModels = -1;
}

void NexuizPlayerModelSelector_loadCvars(entity me)
{
	string skin, modelname;
	float i;

	skin = cvar_string("_cl_playerskin");
	modelname = cvar_string("_cl_playermodel");

	for(i = 0; i < me.numModels; ++i)
	{
		if(bufstr_get(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_MODEL) == modelname)
		if(bufstr_get(me.bufModels, BUFMODELS_COUNT*i+BUFMODELS_SKIN) == skin)
			break;
	}
	if(i >= me.numModels) // fail
		i = 0;
	me.idxModels = i;
}

void NexuizPlayerModelSelector_go(entity me, float d)
{
	me.idxModels = mod(me.idxModels + d + me.numModels, me.numModels);

	if(me.currentModel)
		strunzone(me.currentModel);
	if(me.currentModelTitle)
		strunzone(me.currentModelTitle);
	if(me.currentModelImage)
		strunzone(me.currentModelImage);
	if(me.currentModelDescription)
		strunzone(me.currentModelDescription);

	// select model #i!
	me.currentModelTitle = strzone(bufstr_get(me.bufModels, BUFMODELS_COUNT*me.idxModels+BUFMODELS_TITLE));
	me.currentModelImage = strzone(bufstr_get(me.bufModels, BUFMODELS_COUNT*me.idxModels+BUFMODELS_IMAGE));
	me.currentSkin = stof(bufstr_get(me.bufModels, BUFMODELS_COUNT*me.idxModels+BUFMODELS_SKIN));
	me.currentModel = strzone(bufstr_get(me.bufModels, BUFMODELS_COUNT*me.idxModels+BUFMODELS_MODEL));
	me.currentModelDescription = strzone(bufstr_get(me.bufModels, BUFMODELS_COUNT*me.idxModels+BUFMODELS_DESC));

	// fix the image
	me.src = me.currentModelImage;
	me.updateAspect(me);
}

void PlayerModelSelector_Next_Click(entity btn, entity me)
{
	if (me.numModels <= 0)
		return;
	me.go(me, +1);
	me.saveCvars(me);
}

void PlayerModelSelector_Prev_Click(entity btn, entity me)
{
	if (me.numModels <= 0)
		return;
	me.go(me, -1);
	me.saveCvars(me);
}

void NexuizPlayerModelSelector_saveCvars(entity me)
{
	// we can't immediately apply here because of flood control
	cvar_set("_cl_playermodel", me.currentModel);
	cvar_set("_cl_playerskin", ftos(me.currentSkin));
}

void NexuizPlayerModelSelector_draw(entity me)
{
	float i, n;
	vector o;

	if (me.numModels <= 0)
	{
		draw_CenterText('0.5 0.5 0', "<no model found>", me.realFontSize, '1 1 1', 0.6, FALSE);
		return;
	}

	SUPER(NexuizPlayerModelSelector).draw(me);
	// draw text on the image, handle \n in the description
	draw_CenterText('0.5 0 0', me.currentModelTitle, me.realFontSize * (me.titleFontSize / me.fontSize), SKINCOLOR_MODELTITLE, SKINALPHA_MODELTITLE, FALSE);

	o = '0.5 1 0' - eY * me.realFontSize_y * ((n = tokenizebyseparator(me.currentModelDescription, "\n")) + 0.5);
	for(i = 0; i < n; ++i)
	{
		draw_CenterText(o, argv(i), me.realFontSize, '1 1 1', 1, FALSE);
		o += eY * me.realFontSize_y;
	}
}

void NexuizPlayerModelSelector_resizeNotify(entity me, vector relOrigin, vector relSize, vector absOrigin, vector absSize)
{
	SUPER(NexuizPlayerModelSelector).resizeNotify(me, relOrigin, relSize, absOrigin, absSize);
	me.realFontSize_y = me.fontSize / absSize_y;
	me.realFontSize_x = me.fontSize / absSize_x;
}
#endif
