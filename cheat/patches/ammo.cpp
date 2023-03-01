#include <Windows.h>
#include <iostream>
#include "ammo.h"

bool ammoEnabled = false;

void toggleAmmo() {
  ammoEnabled = !ammoEnabled;
}

void initAmmo() {
  while(1) {
    if (!ammoEnabled)
      continue;

    
  }
}