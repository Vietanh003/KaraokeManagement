import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

public class HashMapExamples {
    // Thành viên 1: Minh họa cơ chế băm và xử lý va chạm
    public static void demonstrateHashing() {
        HashMap<String, Integer> map = new HashMap<>();
        map.put("Key1", 1);
        map.put("Key2", 2);
        map.put("Key1", 3); // Ghi đè, minh họa xử lý va chạm
        System.out.println("HashMap with collision handling: " + map);
    }

    // Thành viên 2: Minh họa cấu trúc (bảng băm, bucket, entry)
    public static void demonstrateStructure() {
        HashMap<String, String> map = new HashMap<>();
        map.put("Apple", "Red");
        map.put("Banana", "Yellow");
        System.out.println("HashMap structure (key-value entries): " + map);
    }

    // Thành viên 3: Minh họa thao tác xóa và tìm kiếm
    public static void demonstrateSearchDelete() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "One");
        map.put(2, "Two");
        System.out.println("Before delete: " + map);
        map.remove(1);
        System.out.println("After delete key 1: " + map);
        System.out.println("Search key 2: " + map.get(2));
    }

    // Thành viên 4: Minh họa duyệt phần tử
    public static void demonstrateIteration() {
        HashMap<String, Double> map = new HashMap<>();
        map.put("Item1", 10.5);
        map.put("Item2", 20.75);
        System.out.println("Iterating HashMap:");
        for (String key : map.keySet()) {
            System.out.println("Key: " + key + ", Value: " + map.get(key));
        }
    }

    // Thành viên 5: So sánh HashMap với List/Array
    public static void demonstrateComparison() {
        // HashMap
        HashMap<String, Integer> map = new HashMap<>();
        map.put("A", 1);
        map.put("B", 2);
        long startTime = System.nanoTime();
        map.get("A");
        long mapTime = System.nanoTime() - startTime;

        // List
        List<String> list = new ArrayList<>();
        list.add("A");
        list.add("B");
        startTime = System.nanoTime();
        list.contains("A");
        long listTime = System.nanoTime() - startTime;

        System.out.println("HashMap get time: " + mapTime + " ns");
        System.out.println("List contains time: " + listTime + " ns");
    }

    public static void main(String[] args) {
        System.out.println("Member 1: Hashing and Collision Handling");
        demonstrateHashing();
        System.out.println("\nMember 2: HashMap Structure");
        demonstrateStructure();
        System.out.println("\nMember 3: Search and Delete");
        demonstrateSearchDelete();
        System.out.println("\nMember 4: Iteration");
        demonstrateIteration();
        System.out.println("\nMember 5: Comparison with List");
        demonstrateComparison();
    }
}
