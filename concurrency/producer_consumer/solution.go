package producer_consumer

import (
	"context"
	"errors"
)

var (
	ErrInvalidCapacity = errors.New("capacity must be positive")
	ErrClosed          = errors.New("queue is closed")
)

type BoundedQueue[T any] struct {
	// TODO: define the queue state and synchronization strategy.
}

func NewBoundedQueue[T any](capacity int) (*BoundedQueue[T], error) {
	// TODO: implement during the interview exercise.
	return nil, nil
}

func (q *BoundedQueue[T]) Put(ctx context.Context, value T) error {
	// TODO: implement during the interview exercise.
	return nil
}

func (q *BoundedQueue[T]) Take(ctx context.Context) (T, error) {
	// TODO: implement during the interview exercise.
	var zero T
	return zero, nil
}

func (q *BoundedQueue[T]) Close() {
	// TODO: implement during the interview exercise.
}
