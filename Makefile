PROGNAME = MarblesSq
ICONTEXT = "Marbles2"
APPID = MarB
VERSION = 1.5
OBJS = grid.o MarblesSq.o MarblesSq-sections.o

#CC=m68k-palmos-gcc -palmos3.5
CC=m68k-palmos-gcc -palmos5
PILRC=pilrc -allowEditID -q
BUILDPRC=build-prc
MULTIGEN = /usr/bin/m68k-palmos-multigen
DEFINES = -DAPPID=$(APPID) -DappId=\'$(APPID)\' -DERROR_CHECK_LEVEL=ERROR_CHECK_FULL -DNON_INTERNATIONAL
CFLAGS = -O2 $(DEFINES) $(INCLUDES)
# -Wall -W -Wno-unused -Wno-switch
#CFLAGS = -O2 -Os -palmos3.5
RESDIR = .
PROD = ./prod
#ZIP = "/program files/winzip/wzzip.exe"
ZIP = zip -j
README = readme.txt
PRCS = $(PROGNAME)_grey.prc $(PROGNAME)_bw.prc $(PROGNAME)_colour.prc

all: $(PROGNAME).prc

colour: $(PROGNAME)_colour.prc

release: src $(README)_update $(PRCS)
	$(ZIP) $(PROD)/$(PROGNAME)-v$(VERSION).zip $(PRCS) $(PROD)/$(README)
	rm -rf $(PROD)/$(README)

src: realclean
	$(ZIP) $(PROD)/$(PROGNAME)_src_v$(VERSION).zip *.c *.h *.bmp *.rcp *.res *.dep Makefile

$(README)_update:
	cat $(README) | sed -e "s/@@$(APPID)_VERSION@@/$(VERSION)/g" | \
	sed -e "s/@@DATE@@/`date +"20%y-%m-%d"`/g" > $(PROD)/$(README)

$(PROGNAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@ $(PROGNAME)-sections.ld

$(PROGNAME)_%.prc: $(PROGNAME).def $(PROGNAME)_%.resstamp $(PROGNAME)
	$(BUILDPRC) $@ $(ICONTEXT) $(APPID) -v $(VERSION) $(PROGNAME) *.bin

$(PROGNAME).prc: $(PROGNAME)_grey.prc

#$(PROGNAME).prc: $(PROGNAME).def $(PROGNAME).resstamp $(PROGNAME)
#	$(BUILDPRC) $@ $(ICONTEXT) $(APPID) -v $(VERSION) $(PROGNAME) *.bin

$(PROGNAME)_%.resstamp: $(RESDIR)/resources_%.rcp $(RESDIR)/*.bmp
	$(PILRC) $(RESDIR)/resources_$*.rcp
	touch $@

$(RESDIR)/resources_%.rcp: $(RESDIR)/$(PROGNAME).rcp
	cat $^ $(RESDIR)/$(PROGNAME)_$*.res | sed -e "s/@@$(APPID)_VERSION@@/$(VERSION)/g" > $@

#$(PROGNAME).resstamp: $(RESDIR)/resources.rcp $(RESDIR)/*.bmp
#	$(PILRC) $(RESDIR)/resources.rcp
#	touch $@

#$(RESDIR)/resources.rcp: $(RESDIR)/$(PROGNAME)_original.rcp
#	cat $^ | sed -e "s/@@$(APPID)_VERSION@@/$(VERSION)/g" > $@

fixed_resources.h: $(PROGNAME).h
	$(CC) $(DEFINES) -E -dD $(PROGNAME).h > $@

.o:
	$(CC) $(CFLAGS) -o $@

$(PROGNAME)-sections.o: $(PROGNAME)-sections.s
	$(CC) -c $(PROGNAME)-sections.s

$(PROGNAME)-sections.s $(PROGNAME)-sections.ld: $(PROGNAME).def
	$(MULTIGEN) $(PROGNAME).def

clean:
	rm -rf *.bin *.o $(PROGNAME) *.ld *.s *.resstamp fixed_resources.h $(RESDIR)/fixed_resources.rcp $(RESDIR)/resources*.rcp

realclean:
	rm -rf *.bin *.o $(PROGNAME) $(PRCS) *.ld *.s *.resstamp fixed_resources.h $(RESDIR)/fixed_resources.rcp $(RESDIR)/resources*.rcp $(ZIPS)
