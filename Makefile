SUBDIRS= src/linux src/windows
.PHONY:	all $(SUBDIRS) clean install uninstall

PLUGIN_DIR=/usr/lib/mozilla/plugins
prefix=/usr/local/
-include config.make

export
all: $(SUBDIRS)	

 $(SUBDIRS):
	$(MAKE) -C $@

install: all
	test -d $(DESTDIR)$(prefix)/share/pipelight || mkdir -p $(DESTDIR)$(prefix)/share/pipelight
	install -m 0644 src/windows/pluginloader.exe $(DESTDIR)$(prefix)/share/pipelight/pluginloader.exe
	sed 's|PLUGIN_LOADER_PATH|$(prefix)/share/pipelight/pluginloader.exe|g' share/pipelight > pipelight.tmp
	install -m 0644 pipelight.tmp $(DESTDIR)$(prefix)/share/pipelight/pipelight
	rm pipelight.tmp
	test -d $(DESTDIR)$(PLUGIN_DIR) || mkdir -p $(DESTDIR)$(PLUGIN_DIR)
	install -m 0644 src/linux/libpipelight.so $(DESTDIR)$(PLUGIN_DIR)/

uninstall:
	rm -f $(DESTDIR)$(prefix)/share/pipelight/pluginloader.exe
	rm -f $(DESTDIR)$(prefix)/share/pipelight/pipelight
	rmdir --ignore-fail-on-non-empty $(DESTDIR)$(prefix)/share/pipelight
	rm -f $(DESTDIR)$(PLUGIN_DIR)/libpipelight.so

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done