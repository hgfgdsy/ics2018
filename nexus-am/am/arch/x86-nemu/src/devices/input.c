#include <am.h>
#include <x86.h>
#include <amdev.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      int t=inl(0x60);
      /*kbd->keydown = 0x1&(t>>15);
      if(kbd->keydown)
	      kbd->keycode=0x8000^t;
      else
      kbd->keycode = t;*/
      /*if((0x1&(t>>15))==1){
      if(kbd->keydown==1){
	      kbd->keycode = _KEY_NONE;
      }
      else{
	      kbd->keydown = 1;
	      kbd->keycode = t^0x8000;
      }
      }
      else{
	      if(kbd->keydown == 1){
		      kbd->keycode = t;
		      kbd->keydown = 0;
	      }
	      else{
		      kbd->keycode = _KEY_NONE;
	      }
      }*/
      kbd->keycode = t;
      if(t){
	      kbd->keydown=0;
      }
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
