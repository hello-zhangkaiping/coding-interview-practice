package two_sum

import "testing"

func TestTwoSum(t *testing.T) {
	got := TwoSum([]int{2, 7, 11, 15}, 9)
	if len(got) != 2 {
		t.Fatalf("expected two indexes, got %v", got)
	}
	if got[0] == got[1] || []int{2, 7, 11, 15}[got[0]]+[]int{2, 7, 11, 15}[got[1]] != 9 {
		t.Fatalf("invalid indexes: %v", got)
	}
}
