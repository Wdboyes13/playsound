package playsound

// #include "c/playsound.h"
import "C"
import "fmt"

func pa_init(nsnds uint64) error {
	if C.pa_init(C.size_t(nsnds)) < 0 {
		return fmt.Errorf("failed to init audio playback")
	}
	return nil
}

func pa_load(path string) (uint64, error) {
	cs := C.CString(path)
	var id C.size_t = 0

	if C.pa_load(cs, &id) < 0 {
		return 0, fmt.Errorf("failed to load sound")
	}

	return uint64(id), nil
}

func pa_play(id uint64) error {
	if C.pa_play(C.size_t(id)) < 0 {
		return fmt.Errorf("failed to start sound")
	}
	return nil
}

func pa_getpos(id uint64) (uint64, error) {
	var pos C.size_t = 0
	if C.pa_getpos(C.size_t(id), &pos) < 0 {
		return 0, fmt.Errorf("failed to get sound position")
	}
	return uint64(pos), nil
}

func pa_setpos(id uint64, pos uint64) error {
	if C.pa_setpos(C.size_t(id), C.size_t(pos)) < 0 {
		return fmt.Errorf("failed to set sound position")
	}
	return nil
}

func pa_stop(id uint64) error {
	if C.pa_stop(C.size_t(id)) < 0 {
		return fmt.Errorf("failed to stop sound")
	}
	return nil
}

func pa_unload(id uint64) error {
	if C.pa_unload(C.size_t(id)) < 0 {
		return fmt.Errorf("failed to unload sound")
	}
	return nil
}

func pa_cleanup() {
	C.pa_cleanup()
}
