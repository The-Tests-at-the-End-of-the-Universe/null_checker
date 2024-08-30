#Compiler and Linker
CC          := cc

#The Target Binary Program
TARGET      := zaphod.so

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := ./src
OBJDIR      := ./build/obj
TARGETDIR   := .
BUILDIR     := ./build
SRCEXT      := c
OBJEXT      := o
TEST        := test.c
TEST_EXE    := test

#Flags, Libraries and Includes
CFLAGS      := -Wall -Werror -Wextra 


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS     := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make

all: $(TARGETDIR)/$(TARGET)

test:
	cc $(TEST) -o $(TEST_EXE)
	LD_PRELOAD=./$(TARGET) ./$(TEST_EXE)

#Remake
re: fclean all

# #Make the Directories
# build:
# 	@mkdir -p $(OBJDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(OBJDIR)

#Full Clean, Objects and Binaries
fclean: clean
	@$(RM) -rf $(BUILDIR)
	@$(RM) -rf $(TARGET)
	@$(RM) -rf test

#Link
$(TARGETDIR)/$(TARGET): 
	echo $(SOURCES)
	gcc -fPIC -shared -o $(TARGET) $(SOURCES) -ldl -pthread

# #Link
# $(TARGETDIR)/$(TARGET) : $(OBJECTS)
# 	$(CC) $(OBJECTS) -o  $(TARGETDIR)/$(TARGET) 

# #Compile
# $(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c -o $@ $^


#Non-File Targets
.PHONY: all re clean fclean


