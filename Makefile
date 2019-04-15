##-----------------------------------------------------------------------------
##
## Copyright (C) 2019 Team Doominati
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## Makefile for Doominati: Shoot-Bang
##
##-----------------------------------------------------------------------------

CC = gdcc-cc
LD = gdcc-ld
NTSC = gdcc-ntsc

CFLAGS = --target-engine=Doominati --no-warn-unused-initializer
LDFLAGS = --target-engine=Doominati


all: codedefs gamedefs gfx maps textdefs


##
## Codedefs
##

shooter_H = \
	shooter/m_defs.h \
	shooter/p_defs.h \
	shooter/r_defs.h

shooter_IR = \
	build/ir/shooter/m_main.ir \
	build/ir/shooter/m_math.ir \
	build/ir/shooter/p_attack.ir \
	build/ir/shooter/p_cond.ir \
	build/ir/shooter/p_edit.ir \
	build/ir/shooter/p_entity.ir \
	build/ir/shooter/p_main.ir \
	build/ir/shooter/p_map.ir \
	build/ir/shooter/p_score.ir \
	build/ir/shooter/p_shop.ir \
	build/ir/shooter/p_spawn.ir \
	build/ir/shooter/p_think.ir \
	build/ir/shooter/r_main.ir \
	build/ir/shooter/r_text.ir

codedefs: build/data/boot/codedefs/libc.bin build/data/boot/codedefs/shooter.bin

build/data/boot/codedefs/libc.bin: | build/data/boot/codedefs
	gdcc-makelib $(CFLAGS) -o$@ libGDCC libc

build/data/boot/codedefs/shooter.bin: $(shooter_IR) | build/data/boot/codedefs
	$(LD) $(LDFLAGS) -o$@ $^

$(shooter_IR) : build/ir/shooter/%.ir : shooter/%.c $(shooter_H) | build/ir/shooter
	$(CC) $(CFLAGS) -co$@ $<


##
## Gamedefs
##

gamedefs: build/data/boot/gamedefs

build/data/boot/gamedefs: gamedefs.txt | build/data/boot
	$(NTSC) --nts-type=GAMEDEFS -o$@ $<


##
## Graphics
##

shooter_gfx = \
	build/data/gfx/Entity/Missile.png \
	build/data/gfx/Entity/Mobj.png \
	build/data/gfx/GUI/DigLarge/20.png \
	build/data/gfx/GUI/DigLarge/0.png \
	build/data/gfx/GUI/DigLarge/1.png \
	build/data/gfx/GUI/DigLarge/2.png \
	build/data/gfx/GUI/DigLarge/3.png \
	build/data/gfx/GUI/DigLarge/4.png \
	build/data/gfx/GUI/DigLarge/5.png \
	build/data/gfx/GUI/DigLarge/6.png \
	build/data/gfx/GUI/DigLarge/7.png \
	build/data/gfx/GUI/DigLarge/8.png \
	build/data/gfx/GUI/DigLarge/9.png \
	build/data/gfx/GUI/DigLarge/A.png \
	build/data/gfx/GUI/DigLarge/C.png \
	build/data/gfx/GUI/DigLarge/E.png \
	build/data/gfx/GUI/DigLarge/H.png \
	build/data/gfx/GUI/DigLarge/L.png \
	build/data/gfx/GUI/DigLarge/P.png \
	build/data/gfx/GUI/DigLarge/U.png \
	build/data/gfx/GUI/DigSmall/20.png \
	build/data/gfx/GUI/DigSmall/0.png \
	build/data/gfx/GUI/DigSmall/1.png \
	build/data/gfx/GUI/DigSmall/2.png \
	build/data/gfx/GUI/DigSmall/3.png \
	build/data/gfx/GUI/DigSmall/4.png \
	build/data/gfx/GUI/DigSmall/5.png \
	build/data/gfx/GUI/DigSmall/6.png \
	build/data/gfx/GUI/DigSmall/7.png \
	build/data/gfx/GUI/DigSmall/8.png \
	build/data/gfx/GUI/DigSmall/9.png \
	build/data/gfx/GUI/DigSmall/C.png \
	build/data/gfx/GUI/DigSmall/L.png \
	build/data/gfx/GUI/Icon/Blank.png \
	build/data/gfx/GUI/Icon/GunFast.png \
	build/data/gfx/GUI/Icon/GunSlow.png \
	build/data/gfx/GUI/Icon/GunWide.png \
	build/data/gfx/GUI/Icon/MagBolt.png \
	build/data/gfx/GUI/Icon/Select1.png \
	build/data/gfx/GUI/Icon/Select2.png \
	build/data/gfx/GUI/Icon/ShopAmmo.png \
	build/data/gfx/GUI/Icon/ShopHeal.png \
	build/data/gfx/GUI/Icon/StatCHA.png \
	build/data/gfx/GUI/Icon/StatEND.png \
	build/data/gfx/GUI/Icon/StatINT.png \
	build/data/gfx/GUI/Icon/StatSTR.png \
	build/data/gfx/GUI/Icon/StatVIT.png \
	build/data/gfx/GUI/Icon/StatWIS.png \
	build/data/gfx/Tile/Edit.png \
	build/data/gfx/Tile/Exit.png \
	build/data/gfx/Tile/Half.png \
	build/data/gfx/Tile/Open.png \
	build/data/gfx/Tile/Wall.png

shooter_gfx_dir = \
	build/data/gfx/Entity \
	build/data/gfx/GUI/DigLarge \
	build/data/gfx/GUI/DigSmall \
	build/data/gfx/GUI/Icon \
	build/data/gfx/Tile

gfx: $(shooter_gfx)

$(shooter_gfx) : build/data/gfx/%.png : gfx/%.png | $(shooter_gfx_dir)
	cp $< $@


##
## Maps
##

shooter_maps = \
	build/data/maps/colinpip \
	build/data/maps/map01 \
	build/data/maps/map02 \
	build/data/maps/map03 \
	build/data/maps/map04 \
	build/data/maps/map05 \
	build/data/maps/map06 \
	build/data/maps/map07

maps: $(shooter_maps)

$(shooter_maps) : build/data/maps/% : maps/%.txt | build/data/maps
	cp $< $@


##
## Textdefs
##

shooter_NTS = \
	build/data/boot/textdefs/shell.nts

textdefs: $(shooter_NTS)

$(shooter_NTS) : build/data/boot/textdefs/%.nts : text/%.txt | build/data/boot/textdefs
	$(NTSC) --nts-type=TEXTDEFS -o$@ $<


##
## Directories
##

build/ir/shooter: | build/ir
	mkdir $@

build/ir: | build
	mkdir $@

build/data/maps: | build/data
	mkdir $@

build/data/gfx/Tile: | build/data/gfx
	mkdir $@

build/data/gfx/GUI/Icon: | build/data/gfx/GUI
	mkdir $@

build/data/gfx/GUI/DigSmall: | build/data/gfx/GUI
	mkdir $@

build/data/gfx/GUI/DigLarge: | build/data/gfx/GUI
	mkdir $@

build/data/gfx/GUI: | build/data/gfx
	mkdir $@

build/data/gfx/Entity: | build/data/gfx
	mkdir $@

build/data/gfx: | build/data
	mkdir $@

build/data/boot/textdefs: | build/data/boot
	mkdir $@

build/data/boot/codedefs: | build/data/boot
	mkdir $@

build/data/boot: | build/data
	mkdir $@

build/data: | build
	mkdir $@

build:
	mkdir $@


##
## Cleanup
##

clean:
	rm -f  $(shooter_maps)
	rm -df build/data/maps
	rm -f  $(shooter_gfx)
	rm -df $(shooter_gfx_dir)
	rm -df build/data/gfx/GUI
	rm -df build/data/gfx
	rm -f  $(shooter_NTS)
	rm -df build/data/boot/textdefs
	rm -f  build/data/boot/gamedefs
	rm -f  build/data/boot/codedefs/shooter.bin
	rm -f  build/data/boot/codedefs/libc.bin
	rm -df build/data/boot/codedefs
	rm -df build/data/boot
	rm -df build/data
	rm -f  $(shooter_IR)
	rm -df build/ir/shooter
	rm -df build/ir
	rm -df build

## EOF

