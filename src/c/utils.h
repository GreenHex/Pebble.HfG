//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

void log_pt( char *str, GPoint pt );
void log_rect( char *str, GRect rect );
void change_layer_colours( GContext *ctx, Layer *layer, GColor old_bg_colour, GColor new_bg_colour, GColor old_fg_colour, GColor new_fg_colour );
void make_outline( GContext *ctx, Layer *layer, GColor fgColour, GColor outlineColor );
