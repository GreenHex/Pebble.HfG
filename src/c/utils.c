//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "utils.h"
#include "global.h"

#define DEBUG

void log_pt( char *str, GPoint pt ) {
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "%s: ( %d, %d )", str, pt.x, pt.y );
  #endif
}

void log_rect( char *str, GRect rect ) {
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "%s: ( %d, %d, %d, %d )", str, rect.origin.x, rect.origin.y, rect.size.w, rect.size.h );
  #endif
}

void change_layer_colours( GContext *ctx, Layer *layer,
                   GColor old_bg_colour, GColor new_bg_colour,
                   GColor old_fg_colour, GColor new_fg_colour ) {
  #if defined( PBL_COLOR )
  // if ( gcolor_equal( old_bg_colour, GColorWhite ) ) return;
  GRect frame = layer_get_frame( layer );
  GPoint origin = layer_convert_point_to_screen( layer, GPointZero );
  GBitmap *fb = graphics_capture_frame_buffer( ctx );
  for( int y = 0; y < frame.size.h + 1; y++ ) {
    GBitmapDataRowInfo row = gbitmap_get_data_row_info( fb, origin.y + y - 1 );    
    for ( int x = 0; x < frame.size.w + 1; x++ ) {
      GColor pixel_colour = (GColor) { .argb = row.data[ origin.x + x ] };
      if ( gcolor_equal( pixel_colour, old_bg_colour ) ) {
        memset( &row.data[ origin.x + x ], new_bg_colour.argb, 1 );        
      } else if ( gcolor_equal( pixel_colour, old_fg_colour ) ) {
        memset( &row.data[ origin.x + x ], new_fg_colour.argb, 1 );
      }
    } 
  }
  graphics_release_frame_buffer( ctx, fb );
  #endif
}

void make_outline( GContext *ctx, Layer *layer, GColor fgColour, GColor outlineColor ) {
  #if defined( PBL_COLOR )
  GRect frame = layer_get_frame( layer );
  GPoint origin = layer_convert_point_to_screen( layer, GPointZero );
  GBitmap *fb = graphics_capture_frame_buffer( ctx );
  
  for( int y = 1; y < frame.size.h - 1; y++ ) {
    GBitmapDataRowInfo r0 = gbitmap_get_data_row_info( fb, origin.y + y - 1 );
    GBitmapDataRowInfo r1 = gbitmap_get_data_row_info( fb, origin.y + y );
    GBitmapDataRowInfo r2 = gbitmap_get_data_row_info( fb, origin.y + y + 1 );
    
    for ( int x = 1; x < frame.size.w - 1; x++ ) {
      GColor c0r0 = (GColor) { .argb = r0.data[ origin.x + x - 1 ] };
      GColor c1r0 = (GColor) { .argb = r0.data[ origin.x + x ] };
      GColor c2r0 = (GColor) { .argb = r0.data[ origin.x + x + 1 ] };
      GColor c0r1 = (GColor) { .argb = r1.data[ origin.x + x - 1 ] };
      GColor c1r1 = (GColor) { .argb = r1.data[ origin.x + x ] };
      GColor c2r1 = (GColor) { .argb = r1.data[ origin.x + x + 1 ] };
      GColor c0r2 = (GColor) { .argb = r2.data[ origin.x + x - 1 ] };
      GColor c1r2 = (GColor) { .argb = r2.data[ origin.x + x ] };
      GColor c2r2 = (GColor) { .argb = r2.data[ origin.x + x + 1 ] };
     
      if ( gcolor_equal( c1r1, GColorWhite ) ) {
        if ( gcolor_equal( c0r0, fgColour ) || gcolor_equal( c1r0, fgColour ) || gcolor_equal( c2r0, fgColour ) ||
            gcolor_equal( c0r1, fgColour ) || gcolor_equal( c2r1, fgColour ) ||
            gcolor_equal( c0r2, fgColour ) || gcolor_equal( c1r2, fgColour ) || gcolor_equal( c2r2, fgColour ) ) {
          memset( &r1.data[ origin.x + x ], outlineColor.argb, 1 );
        }
      }
    } 
  }
  graphics_release_frame_buffer( ctx, fb );
  #endif
}
