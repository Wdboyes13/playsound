package playsound

// #cgo LDFLAGS: -lm
// #include "playsound.h"
import "C"
import "fmt"

func Init(nsnds uint64) error {
	if C.pa_init(C.size_t(nsnds)) < 0 {
		return fmt.Errorf("failed to init audio playback")
	}
	return nil
}

func Load(path string) (uint64, error) {
	cs := C.CString(path)
	var id C.size_t = 0

	if C.pa_load(cs, &id) < 0 {
		return 0, fmt.Errorf("failed to load sound")
	}

	return uint64(id), nil
}

func Play(id uint64) error {
	if C.pa_play(C.size_t(id)) < 0 {
		return fmt.Errorf("failed to start sound")
	}
	return nil
}

func Getpos(id uint64) (uint64, error) {
	var pos C.size_t = 0
	if C.pa_getpos(C.size_t(id), &pos) < 0 {
		return 0, fmt.Errorf("sound not inited")
	}
	return uint64(pos), nil
}

func Setpos(id uint64, pos uint64) error {
	if C.pa_setpos(C.size_t(id), C.size_t(pos)) < 0 {
		return fmt.Errorf("sound not inited")
	}
	return nil
}

func Setloop(id uint64, loop bool) error {
	var ci int = 0
	if loop {
		ci = 1
	}

	if C.pa_setloop(C.size_t(id), C.int(ci)) < 0 {
		return fmt.Errorf("sound not inited")
	}
	return nil
}

func Isplaying(id uint64) (bool, error) {
	var ret int = int(C.pa_isplaying(C.size_t(id)))
	if ret < 0 {
		return false, fmt.Errorf("sound not inited")
	}
	return ret == 1, nil
}

func Stop(id uint64) error {
	if C.pa_stop(C.size_t(id)) < 0 {
		return fmt.Errorf("sound not inited")
	}
	return nil
}

func Unload(id uint64) error {
	if C.pa_unload(C.size_t(id)) < 0 {
		return fmt.Errorf("sound not inited")
	}
	return nil
}

func Cleanup() {
	C.pa_cleanup()
}
