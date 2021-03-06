include $(src)/scripts/utils.mk

bdir:=$(obj)/plugins

PLUGIN_OBJS =
PLUGIN_OBJS += plugin_jbd2.o
PLUGIN_OBJS += plugin_hrtimer.o
PLUGIN_OBJS += plugin_kmem.o
PLUGIN_OBJS += plugin_kvm.o
PLUGIN_OBJS += plugin_mac80211.o
PLUGIN_OBJS += plugin_sched_switch.o
PLUGIN_OBJS += plugin_function.o
PLUGIN_OBJS += plugin_xen.o
PLUGIN_OBJS += plugin_scsi.o
PLUGIN_OBJS += plugin_cfg80211.o
PLUGIN_OBJS += plugin_blk.o
PLUGIN_OBJS += plugin_tlb.o

PLUGIN_OBJS := $(PLUGIN_OBJS:%.o=$(bdir)/%.o)
PLUGINS := $(PLUGIN_OBJS:$(bdir)/%.o=$(bdir)/%.so)
DEPS := $(PLUGIN_OBJS:$(bdir)/%.o=$(bdir)/.%.d)

all: $(PLUGINS)

$(bdir):
	@mkdir -p $(bdir)

$(PLUGIN_OBJS): | $(bdir)
$(DEPS): | $(bdir)

$(PLUGIN_OBJS): $(bdir)/%.o : %.c
	$(Q)$(do_compile_plugin_obj)

$(PLUGINS): $(bdir)/%.so: $(bdir)/%.o
	$(Q)$(do_plugin_build)

$(DEPS): $(bdir)/.%.d: %.c
	$(Q)$(CC) -M $(CPPFLAGS) $(CFLAGS) $< > $@

$(PLUGIN_OBJS): $(bdir)/%.o : $(bdir)/.%.d

$(bdir)/plugin_python.o: $(bdir)/%.o : %.c $(bdir)/trace_python_dir
	$(Q)$(do_compile_python_plugin_obj)

$(bdir)/plugin_python.so: $(bdir)/%.so: $(bdir)/%.o
	$(Q)$(do_python_plugin_build)

PLUGINS_INSTALL = $(subst .so,.install,$(PLUGINS))

$(PLUGINS_INSTALL): $(bdir)/%.install : $(bdir)/%.so force
	$(Q)$(call do_install_data,$<,$(plugin_dir_SQ))

install_plugins: $(PLUGINS_INSTALL)

# The following targets are necessary to trigger a rebuild when $(PLUGIN_DIR)
# and $(PYTHON_DIR) change. Without them, a full clean build would necessary
# in order to get the binaries updated.

$(bdir)/trace_plugin_dir: $(bdir) force
	$(Q)$(N)$(call update_dir, 'PLUGIN_DIR=$(PLUGIN_DIR)')

$(bdir)/trace_python_dir: $(bdir) force
	$(Q)$(N)$(call update_dir, 'PYTHON_DIR=$(PYTHON_DIR)')


dep_includes := $(wildcard $(DEPS))

ifneq ($(dep_includes),)
  include $(dep_includes)
endif

clean:
	$(RM) -f $(bdir)/*.a $(bdir)/*.so $(bdir)/*.o $(bdir)/.*.d $(bdir)/trace_plugin_dir $(bdir)/trace_python_dir

force:
.PHONY: clean force