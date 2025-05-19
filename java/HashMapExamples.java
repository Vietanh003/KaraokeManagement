import java.io.PrintStream;

public class HashMapExamples {
    // Lop Node dai dien cho mot cap key-value
    static class Node {
        Object key;
        Object value;
        int hash;
        Node next;

        Node(Object key, Object value, int hash) {
            this.key = key;
            this.value = value;
            this.hash = hash;
            this.next = null;
        }

        @Override
        public String toString() {
            return "(" + key + ", " + value + ")";
        }
    }

    // Mang bucket
    private Node[] table;
    private int size;
    private static final int DEFAULT_CAPACITY = 16;

    // Constructor
    public HashMapExamples() {
        table = new Node[DEFAULT_CAPACITY];
        size = 0;
    }

    // Ham hash() tu HashMap
    static final int hash(Object key) {
        int hashCode = (key == null) ? 0 : key.hashCode();
        System.out.println("  Buoc 2: hashCode() cua key = " + hashCode);
        int shifted = hashCode >>> 16;
        System.out.println("  Buoc 3: hashCode >>> 16 = " + shifted);
        int result = hashCode ^ shifted;
        System.out.println("  Buoc 4: hash = hashCode ^ (hashCode >>> 16) = " + result);
        return result;
    }

    // Them cap key-value
    public void put(Object key, Object value) {
        System.out.println("\n=== Thao tac put(key = " + key + ", value = " + value + ") ===");
        int hash = hash(key);
        int index = hash & (table.length - 1);
        System.out.println(
                "  Buoc 5: Chi so bucket = hash & (n-1) = " + hash + " & " + (table.length - 1) + " = " + index);

        if (table[index] == null) {
            System.out.println("  Buoc 6: Bucket " + index + " trong, them node moi");
            table[index] = new Node(key, value, hash);
            size++;
        } else {
            System.out.println("  Buoc 6: Bucket " + index + " co node, kiem tra va cham");
            Node current = table[index];
            Node prev = null;

            while (current != null) {
                System.out.println("    Kiem tra node: " + current);
                if (current.hash == hash &&
                        (key == current.key || (key != null && key.equals(current.key)))) {
                    System.out.println("    Key trung, cap nhat value tu " + current.value + " thanh " + value);
                    current.value = value;
                    System.out.println("  Buoc 7: Trang thai mang bucket sau khi cap nhat:");
                    printTable();
                    return;
                }
                prev = current;
                current = current.next;
            }

            System.out.println("    Khong tim thay key trung, them node moi vao danh sach lien ket");
            prev.next = new Node(key, value, hash);
            size++;
        }

        System.out.println("  Buoc 7: Trang thai mang bucket sau khi them:");
        printTable();
    }

    // In trang thai mang bucket
    private void printTable() {
        for (int i = 0; i < table.length; i++) {
            System.out.print("    Bucket " + i + ": ");
            if (table[i] == null) {
                System.out.println("null");
            } else {
                Node current = table[i];
                while (current != null) {
                    System.out.print(current);
                    if (current.next != null) {
                        System.out.print(" -> ");
                    }
                    current = current.next;
                }
                System.out.println();
            }
        }
        System.out.println("    Tong so phan tu: " + size);
    }

    // Main de chay demo co che hoat dong cua HashMap
    public static void main(String[] args) {
        // Bo qua thiet lap UTF-8 vi khong can dau tieng Viet
        HashMapExamples map = new HashMapExamples();
        System.out.println("\n=== Demo co che hoat dong cua HashMap ===");
        System.out.println("Minh hoa ham bam, xu ly va cham (danh sach lien ket):");
        map.put("apple", "Red");
        map.put("banana", "Yellow");
        map.put("cherry", "Pink"); // Gay va cham
        System.out.println("Luu y: Mo rong khong duoc trien khai trong demo nay.");
    }
}