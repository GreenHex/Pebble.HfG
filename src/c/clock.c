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
  graphics_context_set_fill_color( ctx, background_colour );
  graphics_fill_rect( ctx, bounds, CLOCK_CORNER_RADIUS, GCornersAll );
}

static void hours_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
}

static void minutes_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
}

void clock_init( Window* window ){ 
  window_layer = window_get_root_layer( window );
  
  foreground_colour = BACKGROUND_COLOUR;
  background_colour = FOREGROUND_COLOUR;
  
  dial_layer = layer_create( CLOCK_DIAL_RECT );
  layer_set_update_proc( dial_layer, dial_layer_update_proc );
  layer_add_child( window_layer, dial_layer );
  GRect dial_layer_bounds = layer_get_bounds( dial_layer ); 
    
  hours_layer = layer_create( dial_layer_bounds );
  layer_set_update_proc( hours_layer, hours_layer_update_proc );
  layer_add_child( dial_layer, hours_layer );
  
  minutes_layer = layer_create( dial_layer_bounds );
  layer_set_update_proc( minutes_layer, minutes_layer_update_proc );
  layer_add_child( dial_layer, minutes_layer );

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  time_t now = time( NULL );
  handle_clock_tick( localtime( &now ), 0 );
}

void clock_deinit( void ) {
  if ( minutes_layer ) layer_destroy( minutes_layer );
  if ( hours_layer ) layer_destroy( hours_layer );
  if ( dial_layer ) layer_destroy( dial_layer );
}
