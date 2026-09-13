package reverse_linked_list

import "testing"

func TestReverseList(t *testing.T) {
	head := &ListNode{Val: 1, Next: &ListNode{Val: 2, Next: &ListNode{Val: 3}}}
	got := ReverseList(head)
	want := []int{3, 2, 1}
	for _, value := range want {
		if got == nil || got.Val != value {
			t.Fatalf("unexpected reversed list at value %d", value)
		}
		got = got.Next
	}
	if got != nil {
		t.Fatal("reversed list contains extra nodes")
	}
}
