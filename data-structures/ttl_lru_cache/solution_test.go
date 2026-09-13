package ttl_lru_cache

import (
	"testing"
	"time"
)

type fakeClock struct{ current time.Time }

func (f *fakeClock) Now() time.Time          { return f.current }
func (f *fakeClock) Advance(d time.Duration) { f.current = f.current.Add(d) }

func TestTTLLRUBasicGet(t *testing.T) {
	clock := &fakeClock{current: time.Unix(0, 0)}
	cache, err := NewTTLLRU(2, clock.Now)
	if err != nil {
		t.Fatalf("NewTTLLRU returned error: %v", err)
	}
	if err := cache.Put("a", 1, time.Minute); err != nil {
		t.Fatalf("Put returned error: %v", err)
	}
	if value, ok := cache.Get("a"); !ok || value != 1 {
		t.Fatalf("Get(a) = (%d, %v), want (1, true)", value, ok)
	}
}

func TestTTLLRUExpiration(t *testing.T) {
	clock := &fakeClock{current: time.Unix(0, 0)}
	cache, err := NewTTLLRU(2, clock.Now)
	if err != nil {
		t.Fatalf("NewTTLLRU returned error: %v", err)
	}
	if err := cache.Put("a", 1, time.Second); err != nil {
		t.Fatalf("Put returned error: %v", err)
	}
	clock.Advance(time.Second)
	if _, ok := cache.Get("a"); ok {
		t.Fatal("expired key should miss")
	}
	if cache.Len() != 0 {
		t.Fatalf("Len() = %d, want 0", cache.Len())
	}
}

func TestTTLLRUEvictionOrder(t *testing.T) {
	clock := &fakeClock{current: time.Unix(0, 0)}
	cache, err := NewTTLLRU(2, clock.Now)
	if err != nil {
		t.Fatalf("NewTTLLRU returned error: %v", err)
	}
	_ = cache.Put("a", 1, time.Hour)
	_ = cache.Put("b", 2, time.Hour)
	if _, ok := cache.Get("a"); !ok {
		t.Fatal("expected a to exist")
	}
	_ = cache.Put("c", 3, time.Hour)
	if _, ok := cache.Get("b"); ok {
		t.Fatal("b should be evicted as the least recently used key")
	}
}
