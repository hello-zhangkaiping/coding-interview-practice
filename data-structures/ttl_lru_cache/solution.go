package ttl_lru_cache

import (
	"errors"
	"time"
)

var (
	ErrInvalidCapacity = errors.New("capacity must be positive")
	ErrInvalidTTL      = errors.New("ttl must be positive")
)

type TTLLRU struct {
	// TODO: define the state and invariants.
}

func NewTTLLRU(capacity int, now func() time.Time) (*TTLLRU, error) {
	// TODO: implement during the interview exercise.
	return nil, nil
}

func (c *TTLLRU) Put(key string, value int, ttl time.Duration) error {
	// TODO: implement during the interview exercise.
	return nil
}

func (c *TTLLRU) Get(key string) (int, bool) {
	// TODO: implement during the interview exercise.
	return 0, false
}

func (c *TTLLRU) Len() int {
	// TODO: implement during the interview exercise.
	return 0
}
