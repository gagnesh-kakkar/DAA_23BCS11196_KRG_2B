package DAA;

class MyStack<T> {
    private int capacity;
    private int top;
    private T[] stack;

    public MyStack(int size) {
        capacity = size;
        top = -1;
        stack = (T[]) new Object[size];
    }

    public void push(T element) {
        if (isFull()) {
            System.out.println("Stack Overflow! Cannot push " + element);
        } else {
            stack[++top] = element;
            System.out.println(element + " pushed into stack.");
        }
    }

    public T pop() {
        if (isEmpty()) {
            System.out.println("Stack Underflow! Cannot pop.");
            return null;
        } else {
            return stack[top--];
        }
    }

    public T peek() {
        if (isEmpty()) {
            System.out.println("Stack is empty. No top element.");
            return null;
        } else {
            return stack[top];
        }
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public boolean isFull() {
        return top == capacity - 1;
    }
}

class StackDemo {
    public static void main(String[] args) {
        MyStack<Integer> intStack = new MyStack<>(5);

        intStack.push(10);
        intStack.push(20);
        intStack.push(30);

        System.out.println("Top element: " + intStack.peek());

        System.out.println("Popped: " + intStack.pop());
        System.out.println("Popped: " + intStack.pop());

        System.out.println("Is stack empty? " + intStack.isEmpty());
        System.out.println("Is stack full? " + intStack.isFull());
    }
}

