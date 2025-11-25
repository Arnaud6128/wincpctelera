//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Deeper Warrens
//  Copyright (C) 2018 Arnaud Bouche (@Arnaud6128)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

//#####################################################################
//### MODULE: Sprites
//### SUBMODULE: CheckCollision
//#####################################################################
//### This module contains routines to check collisions
//#####################################################################

#ifndef CPCT_CHECKCOLLISION_H
#define CPCT_CHECKCOLLISION_H

// Functions to check collision in 1D
extern u8 cpct_checkCollisionAxis(u8 x1, u8 w1, u8 x2, u8 w2) __z88dk_callee;

// Functions to check collision in 2D
extern u8 cpct_checkCollisionBoxes(u8 x1, u8 w1, u8 y1, u8 h1, u8 x2, u8 w2, u8 y2, u8 h2) __z88dk_callee;

#endif