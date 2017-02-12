//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

// #define DEBUG

#define CLOCK_DIAL_SIZE_W 144
#define CLOCK_DIAL_SIZE_H 168
#define CLOCK_DIAL_POS_X 0
#define CLOCK_DIAL_POS_Y 0
#define CLOCK_DIAL_FRAME ( GRect( CLOCK_DIAL_POS_X, CLOCK_DIAL_POS_Y, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )
#define GUTTER 4

#define HOURS_SIZE_W ( CLOCK_DIAL_SIZE_W / 2 - GUTTER )
#define HOURS_SIZE_H 40
#define HOURS_POS_X ( CLOCK_DIAL_SIZE_W / 2 - HOURS_SIZE_W - GUTTER )
#define HOURS_POS_Y ( CLOCK_DIAL_SIZE_H / 2 - HOURS_SIZE_H + 20 )
#define HOURS_FRAME ( GRect( HOURS_POS_X, HOURS_POS_Y, HOURS_SIZE_W, HOURS_SIZE_H ) )

#define MINUTES_SIZE_W HOURS_SIZE_W
#define MINUTES_SIZE_H 58
#define MINUTES_POS_X HOURS_POS_X
#define MINUTES_POS_Y ( HOURS_POS_Y + HOURS_SIZE_H )
#define MINUTES_FRAME ( GRect( MINUTES_POS_X, MINUTES_POS_Y, MINUTES_SIZE_W, MINUTES_SIZE_H ) )

#define DATE_SIZE_W HOURS_SIZE_W
#define DATE_SIZE_H 14
#define DATE_POS_X ( HOURS_POS_X + HOURS_SIZE_W + 2 * GUTTER )
#define DATE_POS_Y ( HOURS_POS_Y + HOURS_SIZE_H - DATE_SIZE_H )
#define DATE_FRAME ( GRect( DATE_POS_X, DATE_POS_Y, DATE_SIZE_W, DATE_SIZE_H ) )

#define G1_0 ( GPoint( HOURS_POS_X + HOURS_SIZE_W, 0 ) )
#define G1_1 ( GPoint( HOURS_POS_X + HOURS_SIZE_W, PBL_DISPLAY_HEIGHT ) )
#define G2_0 ( GPoint( HOURS_POS_X + HOURS_SIZE_W + 2*GUTTER, 0 ) )
#define G2_1 ( GPoint( HOURS_POS_X + HOURS_SIZE_W + 2*GUTTER, PBL_DISPLAY_HEIGHT ) )

/*
RESOURCE_ID_FONT_PRELUDE_BOLD_32
RESOURCE_ID_FONT_PRELUDE_BOLD_36
RESOURCE_ID_FONT_PRELUDE_BOLD_40
RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_ITALIC_48
RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_14
RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_36
RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_48
RESOURCE_ID_FONT_PRELUDE_MEDIUM_36
RESOURCE_ID_FONT_PRELUDE_MEDIUM_48
RESOURCE_ID_FONT_PRELUDE_MEDIUM_OBLIQUE_14
*/

// #define SHOW_GRID
#define HOUR_FONT      RESOURCE_ID_FONT_PRELUDE_BOLD_36
#define MINUTE_FONT    RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_48
#define DATE_FONT      RESOURCE_ID_FONT_PRELUDE_COND_LIGHT_14
#define HOUR_TEXT_VER_ADJ -3
#define MINUTE_TEXT_VER_ADJ 8
#define DATE_TEXT_VER_ADJ 2

#define FOREGROUND_COLOUR PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define BACKGROUND_COLOUR PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )

extern tm tm_time;
extern GColor foreground_colour;
extern GColor background_colour;
