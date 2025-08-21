package DAA;

// Node for Doubly Linked List
class DoublyNode {
    int data;
    DoublyNode prev, next;
    public DoublyNode(int data) {
        this.data = data;
        prev = next = null;
    }
}

// Node for Circular Linked List
class CircularNode {
    int data;
    CircularNode next;
    public CircularNode(int data) {
        this.data = data;
        next = null;
    }
}

class LinkedListDemo {

    // ====== DOUBLY LINKED LIST ======
    DoublyNode headDLL;

    void insertAtBeginningDLL(int data) {
        DoublyNode newNode = new DoublyNode(data);
        if (headDLL != null) {
            newNode.next = headDLL;
            headDLL.prev = newNode;
        }
        headDLL = newNode;
    }

    void insertAtEndDLL(int data) {
        DoublyNode newNode = new DoublyNode(data);
        if (headDLL == null) {
            headDLL = newNode;
            return;
        }
        DoublyNode temp = headDLL;
        while (temp.next != null) temp = temp.next;
        temp.next = newNode;
        newNode.prev = temp;
    }

    void deleteAtBeginningDLL() {
        if (headDLL == null) return;
        headDLL = headDLL.next;
        if (headDLL != null) headDLL.prev = null;
    }

    void deleteAtEndDLL() {
        if (headDLL == null) return;
        if (headDLL.next == null) {
            headDLL = null;
            return;
        }
        DoublyNode temp = headDLL;
        while (temp.next != null) temp = temp.next;
        temp.prev.next = null;
    }

    void displayDLL() {
        DoublyNode temp = headDLL;
        while (temp != null) {
            System.out.print(temp.data + " ");
            temp = temp.next;
        }
        System.out.println();
    }

    // ====== CIRCULAR LINKED LIST ======
    CircularNode headCLL;

    void insertAtBeginningCLL(int data) {
        CircularNode newNode = new CircularNode(data);
        if (headCLL == null) {
            newNode.next = newNode;
            headCLL = newNode;
            return;
        }
        CircularNode temp = headCLL;
        while (temp.next != headCLL) temp = temp.next;
        newNode.next = headCLL;
        temp.next = newNode;
        headCLL = newNode;
    }

    void insertAtEndCLL(int data) {
        CircularNode newNode = new CircularNode(data);
        if (headCLL == null) {
            newNode.next = newNode;
            headCLL = newNode;
            return;
        }
        CircularNode temp = headCLL;
        while (temp.next != headCLL) temp = temp.next;
        temp.next = newNode;
        newNode.next = headCLL;
    }

    void deleteAtBeginningCLL() {
        if (headCLL == null) return;
        if (headCLL.next == headCLL) {
            headCLL = null;
            return;
        }
        CircularNode temp = headCLL;
        while (temp.next != headCLL) temp = temp.next;
        headCLL = headCLL.next;
        temp.next = headCLL;
    }

    void deleteAtEndCLL() {
        if (headCLL == null) return;
        if (headCLL.next == headCLL) {
            headCLL = null;
            return;
        }
        CircularNode temp = headCLL;
        while (temp.next.next != headCLL) temp = temp.next;
        temp.next = headCLL;
    }

    void displayCLL() {
        if (headCLL == null) return;
        CircularNode temp = headCLL;
        do {
            System.out.print(temp.data + " ");
            temp = temp.next;
        } while (temp != headCLL);
        System.out.println();
    }

    public static void main(String[] args) {
        LinkedListDemo list = new LinkedListDemo();

        System.out.println("Doubly Linked List Operations:");
        list.insertAtBeginningDLL(10);
        list.insertAtEndDLL(20);
        list.insertAtBeginningDLL(5);
        list.displayDLL();
        list.deleteAtEndDLL();
        list.displayDLL();

        System.out.println("\nCircular Linked List Operations:");
        list.insertAtBeginningCLL(10);
        list.insertAtEndCLL(20);
        list.insertAtBeginningCLL(5);
        list.displayCLL();
        list.deleteAtEndCLL();
        list.displayCLL();
    }
}
