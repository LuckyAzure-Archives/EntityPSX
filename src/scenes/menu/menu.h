#ifndef PSXF_GUARD_MENU_H
#define PSXF_GUARD_MENU_H

#include "../stage/stage.h"

//Menu enums
typedef enum
{
	MenuPage_Opening,
	MenuPage_Title,
	MenuPage_Main,
	MenuPage_Story,
	MenuPage_Freeplay,
	MenuPage_Mods,
	MenuPage_Options,
	
	MenuPage_Stage, //Changes game loop
} MenuPage;

//Menu state
typedef struct
{
	//Menu state
	u8 page, next_page;
	boolean page_swap;
	u8 select, next_select;
	s16 animcounter;
	
	fixed_t scroll;
	fixed_t trans_time;
	
	//Page specific state
	union
	{
		struct
		{
			fixed_t logo_bump;
			fixed_t fade, fadespd;
		} title;
		struct
		{
			fixed_t fade, fadespd;
		} story;
		struct
		{
			fixed_t back_r, back_g, back_b;
		} freeplay;
	} page_state;
	
	union
	{
		struct
		{
			u8 id, diff;
			boolean story;
		} stage;
	} page_param;
	
	//Menu assets
	Gfx_Tex tex_back, tex_ng, tex_story, tex_title, tex_options;
	Gfx_Tex tex_titleback0, tex_titleback1, tex_titleback2, tex_titleback3;
	FontData font_bold, font_arial;
	
	Character *gf; //Title Girlfriend
} Menu;

extern Menu menu;

//Menu functions
void Menu_Load(MenuPage page);
void Menu_Unload();
void Menu_ToStage(StageId id, StageDiff diff, boolean story);
void Menu_Tick();

#endif