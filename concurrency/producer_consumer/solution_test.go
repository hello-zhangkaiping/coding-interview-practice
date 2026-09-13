package producer_consumer

import (
	"context"
	"errors"
	"testing"
)

func TestBoundedQueueFIFO(t *testing.T) {
	queue, err := NewBoundedQueue[int](2)
	if err != nil {
		t.Fatalf("NewBoundedQueue returned error: %v", err)
	}
	ctx := context.Background()
	if err := queue.Put(ctx, 1); err != nil {
		t.Fatalf("Put returned error: %v", err)
	}
	if err := queue.Put(ctx, 2); err != nil {
		t.Fatalf("Put returned error: %v", err)
	}
	for _, want := range []int{1, 2} {
		got, err := queue.Take(ctx)
		if err != nil || got != want {
			t.Fatalf("Take() = (%d, %v), want (%d, nil)", got, err, want)
		}
	}
}

func TestBoundedQueueDrainsAfterClose(t *testing.T) {
	queue, err := NewBoundedQueue[int](1)
	if err != nil {
		t.Fatalf("NewBoundedQueue returned error: %v", err)
	}
	ctx := context.Background()
	if err := queue.Put(ctx, 7); err != nil {
		t.Fatalf("Put returned error: %v", err)
	}
	queue.Close()
	if got, err := queue.Take(ctx); err != nil || got != 7 {
		t.Fatalf("first Take() = (%d, %v), want (7, nil)", got, err)
	}
	if _, err := queue.Take(ctx); !errors.Is(err, ErrClosed) {
		t.Fatalf("second Take() error = %v, want ErrClosed", err)
	}
}
