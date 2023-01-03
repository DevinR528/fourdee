
# vim: set tabstop=4 smarttab

CC ?= gcc

buildtype ?= release

CPPFLAGS += -Wfatal-errors

CPPFLAGS += -I ./src
CPPFLAGS += -I /usr/include/freetype2
CPPFLAGS += -I ./includes

CXXFLAGS += -Wall -Wextra -Werror
CXXFLAGS += -std=c++2a

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CXXFLAGS += -O2
CXXFLAGS += -flto

# LDFLAGS += -static
else
CPPFLAGS += -D DEBUGGING
CXXFLAGS += -O0
CXXFLAGS += -g
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Wno-unused-parameter
endif

LDLIBS += -lstdc++ -lGL -lglfw -lm -lfreetype

on_error ?= do_nothing

ifeq ($(on_error), do_nothing)
ON_ERROR =
else ifeq ($(on_error), open_editor)
ON_ERROR += || ($$EDITOR $<; false)
else
$(error "invalid on_error option!");
endif

buildprefix ?= build/cc-$(CC)/$(buildtype)


all: $(buildprefix)/fordee

.PRECIOUS: %/

%/:
	@ mkdir -p $@

build/srclist.mk: | build/
	find -name '*.cpp' ! -path '*/old/*' | sed 's/^/srcs += /' > $@

include build/srclist.mk

# ARGS += -i examples/foobar.zd

run: $(buildprefix)/fordee
	$< $(ARGS)

valrun: $(buildprefix)/fordee
	valgrind --suppressions=./stl-val.supp --track-origins=yes --keep-debuginfo=yes --leak-check=full $< $(ARGS)

$(buildprefix)/fordee: $(patsubst %.cpp,$(buildprefix)/%.o,$(srcs))
	@ echo "linking $@"
	@ echo $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(buildprefix)/%.o $(buildprefix)/%.d: %.cpp | $(buildprefix)/%/
	@ echo "compiling $<"
	@ echo $(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -MD -o $(buildprefix)/$*.o $(ON_ERROR)
	@ $(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -MD -o $(buildprefix)/$*.o $(ON_ERROR)

format:
	find -name '*.cpp' | xargs -d \\n clang-format-11 -i --verbose

clean:
	rm -rf build

distclean: clean
	# Let me keep my vscode project level config
	for l in $$(cat .gitignore); do if [ $$l != ".vscode" ]; then rm -rf $$l; fi done







