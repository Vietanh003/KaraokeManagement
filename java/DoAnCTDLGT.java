/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.doanctdlgt;

/**
 *
 * @author HOME
 */
import java.util.HashMap;
import java.util.Map;

public class DoAnCTDLGT {
    public static void main(String[] args) {
        // Khởi tạo HashMap với key kiểu String và value kiểu Integer
        Map<String, Integer> studentScores = new HashMap<>();

        // Thêm phần tử vào HashMap
        studentScores.put("An", 85);
        studentScores.put("Binh", 90);
        studentScores.put("Cuong", 78);

        // Ghi đè giá trị nếu key đã tồn tại
        studentScores.put("An", 95);

        // Duyệt và in các phần tử
        for (Map.Entry<String, Integer> entry : studentScores.entrySet()) {
            System.out.println("Ten: " + entry.getKey() + ", Diem: " + entry.getValue());
        }
    }
}
