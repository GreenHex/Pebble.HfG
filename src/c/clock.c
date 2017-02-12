//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"
#include "clock.h"

tm tm_time;
GColor foreground_colour;
GColor background_colour;

static Layer *window_layer = 0;
static Layer *dial_layer = 0;
static Layer *hours_layer = 0;
static Layer *minutes_layer = 0;
static Layer *date_layer = 0;
static GFont hour_font;
static GFont minute_font;
static GFont date_font;

static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH( two_segments ),
};

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
  
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %02d:%02d:%02d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );
  #endif
  
  layer_mark_dirty( dial_layer );
  if ( ( units_changed & HOUR_UNIT ) && ( !quiet_time_is_active() ) ) vibes_enqueue_custom_pattern( double_vibe_pattern );
}

static void dial_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  graphics_context_set_antialiased( ctx, true );
  graphics_context_set_fill_color( ctx, GColorBlue );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  #ifdef SHOW_GRID
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_line( ctx, G1_0, G1_1 );
  graphics_draw_line( ctx, G2_0, G2_1 );
  #endif
}

static void hours_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  #ifdef SHOW_GRID
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_fill_color( ctx, GColorRed );
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  // graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  #endif
  bounds.origin.y -= HOUR_TEXT_VER_ADJ;
  char str[] = "##";
  strftime( str, sizeof( str ), clock_is_24h_style() ?  "%H" : "%I", &tm_time );
  if ( str[0] == '0' ) {
    memmove( &str[0], &str[1], sizeof( str ) - 1 );
  }
  graphics_context_set_text_color( ctx, PBL_IF_COLOR_ELSE( GColorPastelYellow, GColorWhite ) );
  graphics_draw_text( ctx, str, hour_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL );                              
}

static void minutes_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  #ifdef SHOW_GRID
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_fill_color( ctx, GColorGreen );
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  // graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  #endif
  bounds.origin.y -= MINUTE_TEXT_VER_ADJ;
  char str[] = "##";
  strftime( str, sizeof( str ), "%M", &tm_time );
  graphics_context_set_text_color( ctx, PBL_IF_COLOR_ELSE( GColorPastelYellow, GColorWhite ) );
  graphics_draw_text( ctx, str, minute_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL );
}

static void date_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  #ifdef SHOW_GRID
  GPoint center_pt = grect_center_point( &bounds );
  graphics_context_set_fill_color( ctx, GColorYellow );
  graphics_context_set_stroke_color( ctx, GColorLightGray );
  graphics_draw_rect( ctx, bounds );
  // graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  #endif
  bounds.origin.y -= DATE_TEXT_VER_ADJ;
  char str[] = "AAA DD, YYYY";
  strftime( str, sizeof( str ), "%A %e", &tm_time );
  // for(char*p=str;*p;++p) *p=*p>0x40&&*p<0x5b?*p|0x60:*p; // http://stackoverflow.com/questions/2661766/c-convert-a-mixed-case-string-to-all-lower-case
  graphics_context_set_text_color( ctx, PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite ) );
  graphics_draw_text( ctx, str, date_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL );
}

void clock_init( Window* window ){ 
  window_layer = window_get_root_layer( window );
  
  foreground_colour = BACKGROUND_COLOUR;
  background_colour = FOREGROUND_COLOUR;
  
  dial_layer = layer_create( CLOCK_DIAL_FRAME );
  layer_set_update_proc( dial_layer, dial_layer_update_proc );
  layer_add_child( window_layer, dial_layer );
  GRect dial_layer_bounds = layer_get_bounds( dial_layer ); 
    
  hours_layer = layer_create( HOURS_FRAME );
  layer_set_update_proc( hours_layer, hours_layer_update_proc );
  layer_add_child( dial_layer, hours_layer );
  
  minutes_layer = layer_create( MINUTES_FRAME );
  layer_set_update_proc( minutes_layer, minutes_layer_update_proc );
  layer_add_child( dial_layer, minutes_layer );
  
  date_layer = layer_create( DATE_FRAME );
  layer_set_update_proc( date_layer, date_layer_update_proc );
  layer_add_child( dial_layer, date_layer );

  hour_font = fonts_load_custom_font( resource_get_handle( HOUR_FONT ) );
  minute_font = fonts_load_custom_font( resource_get_handle( MINUTE_FONT ) );
  date_font = fonts_load_custom_font( resource_get_handle( DATE_FONT ) );
  
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  time_t now = time( NULL );
  handle_clock_tick( localtime( &now ), 0 );
}

void clock_deinit( void ) {
  fonts_unload_custom_font( hour_font );
  fonts_unload_custom_font( minute_font );
  fonts_unload_custom_font( date_font );
  if ( minutes_layer ) layer_destroy( minutes_layer );
  if ( hours_layer ) layer_destroy( hours_layer );
  if ( dial_layer ) layer_destroy( dial_layer );
}
