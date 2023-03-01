CXX = g++
CXXFLAGS = -static

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build targets
build:
	$(CXX) $(CXXFLAGS) -DUNICODE -I./cheat -I./cheat/patches $(wildcard ./cheat/*.cpp) $(wildcard ./cheat/**/*.cpp) ./no_inject.cpp -o no_inject.exe
build_dll:
	$(CXX) $(CXXFLAGS) -DUNICODE ./injector/injector.cpp -o injector.exe
	$(CXX) $(CXXFLAGS) -c -I ./cheat $(wildcard ./cheat/*.cpp) $(wildcard ./cheat/**/*.cpp)
	$(CXX) $(CXXFLAGS) -mdll -o half_lunch.dll ammo.o patch_memory.o main.o -Wl,--out-implib,half_lunch.dll -L.
clean:
	rm -f injector.exe half_lunch.dll
	find . -name "*.o" -type f -delete
