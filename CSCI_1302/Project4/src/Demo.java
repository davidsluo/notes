import LinkedList.SortedList;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * A demo of SortedList using Person and Student objects
 *
 * @author David Luo
 */
public class Demo {


    public static void main(String[] args) {

        // Random data to test with
        Student benson = new Student("Benson", "Chau", 1, convertToDate(3, 13, 1978), "UGA");
        Student frank = new Student("Frank", "Smith", 2, convertToDate(1, 1, 2842), "Harvard");
        Student hawking = new Student("Stephen", "Hawking", 3, convertToDate(3, 2, 2981), "MIT");

        Person grandpa = new Person("Kyle", "Grandpa", 238, convertToDate(1, 1, 1997));
        Person rick = new Person("Rick", "Astley", 3841, convertToDate(2, 6, 1966));

        // Regular unsorted arrays for convenience.
        Person[] peopleArr = {
                grandpa, rick, benson, frank, hawking
        };
        Student[] studentsArr = {
                benson, hawking, frank
        };

        Person[] onlyPeopleArr = {
                grandpa, rick
        };
        Student[] onlyStudentsArr = {
                benson, frank, hawking
        };
        // Arbitrary scope
        {
            // studentList is a prefix of personList when sorted by ID
            SortedList<Person> peopleList = new SortedList<>();
            SortedList<Student> studentList = new SortedList<>();

            // Add data to lists
            System.out.println("Sorting");
            System.out.println("-------");
            System.out.println("Unordered People");
            for (Person p : peopleArr)
                System.out.println(p.toString());

            System.out.println("Unordered Students");
            for (Student s : studentsArr)
                System.out.println(s.toString());

            fromArrayToList(peopleArr, peopleList);
            fromArrayToList(studentsArr, studentList);

            System.out.println();

            // Test printList()
            System.out.println("Ordered People");
            peopleList.printList();
            System.out.println("Ordered Students");
            studentList.printList();

            System.out.println();

            // Testing rejection of duplicates and null objects
            System.out.println("Rejection of duplicate and null objects");
            System.out.println("---------------------------------------");
            System.out.println("Attempting to add duplicate entry: ");
            System.out.println(benson.toString());
            System.out.println("Succeeded?");
            System.out.println(peopleList.add(benson));

            System.out.println("Attempting to add null entry");
            System.out.println("Succeeded?");
            System.out.println(peopleList.add(null));

            System.out.println();

            // Test prefix
            System.out.println("isPrefix");
            System.out.println("--------");
            System.out.println("Students prefix of People?");
            System.out.println(studentList.isPrefix(peopleList));
        }

        System.out.println();

        {
            SortedList<Student> onlyStudents = new SortedList<>();
            SortedList<Person> onlyPeople = new SortedList<>();

            fromArrayToList(onlyStudentsArr, onlyStudents);
            fromArrayToList(onlyPeopleArr, onlyPeople);

            // Test merge
            System.out.println("Merging");
            System.out.println("-------");
            System.out.println("Just People");
            onlyPeople.printList();
            System.out.println("Just Students");
            onlyStudents.printList();

            SortedList<Person> merged = onlyPeople.merge(onlyStudents);

            System.out.println("Merged");
            merged.printList();
        }

        System.out.println();

        {
            SortedList<Person> allPeople = new SortedList<>();

            fromArrayToList(peopleArr, allPeople);

            Person bob = new Person("Bob", "Joe", 1254, convertToDate(1, 1, 3829));

            System.out.println("This is bob: ");
            System.out.println(bob.toString());

            System.out.println();

            //Testing adding
            System.out.println("Adding object to list");
            System.out.println("---------------------");
            System.out.println("A list:");
            allPeople.printList();
            System.out.println("Adding Bob");
            allPeople.add(bob);
            System.out.println("The List again:");
            allPeople.printList();

            System.out.println();

            //Testing indexOf
            System.out.println("Getting index of object");
            System.out.println("-----------------------");
            System.out.println("Getting index of Bob");
            System.out.println(allPeople.indexOf(bob));

            System.out.println();

            //Testing removing
            System.out.println("Removing element from list");
            System.out.println("--------------------------");
            System.out.println("Removing Bob");
            allPeople.remove(bob);
            System.out.println("The List again:");
            allPeople.printList();

            System.out.println();

            //Testing Size
            System.out.println("Getting Size");
            System.out.println("------------");
            System.out.println("Size of list: ");
            System.out.println(allPeople.size());

            System.out.println();

            //Testing get
            System.out.println("Getting by index");
            System.out.println("----------------");
            for (int i = 0; i < allPeople.size(); i++) {
                System.out.println("Index: " + i);
                System.out.println(allPeople.get(i).toString());
            }

            System.out.println();

            //Testing isEmpty
            System.out.println("isEmpty");
            System.out.println("-------");
            System.out.println("Is list empty?");
            System.out.println(allPeople.isEmpty());
            System.out.println("Removing everything in list.");
            for (Person p : allPeople) {
                System.out.println("Removing: " + p.toString());
                allPeople.remove(p);
            }
            System.out.println("The list:");

            System.out.println();

            allPeople.printList();
            System.out.println("Is list empty?");
            System.out.println(allPeople.isEmpty());
        }

        System.out.println();

        {
            SortedList<Person> original = new SortedList<>();
            fromArrayToList(peopleArr, original);

            System.out.println("Serialization");
            System.out.println("-------------");
            System.out.println("Original list:");
            original.printList();
            System.out.println("Serializing to file named \"serialized\"");
            SortedList.serialze(original);
            System.out.println("Creating new list from deserialized file.");

            SortedList<Person> deserialized = SortedList.deserialize();

            System.out.println("The deserialized list:");
            deserialized.printList();

        }

        System.out.println();

        System.out.println("Demo finished.");
    }


    // This is probably ugly.
    public static void fromArrayToList(Object[] arr, SortedList list) {

        for (Object obj : arr) {
            try {
                if (!list.add((Comparable) obj)) {
                    System.out.println("Failed to add: " + obj.toString());
                }
            } catch (Exception e) {
                return;
            }
        }
    }

    // This is probably uglier.
    private static Date convertToDate(int month, int day, int year) {
        String date = String.format("%02d-%02d-%04d", month, day, year);

        SimpleDateFormat sdf = new SimpleDateFormat("MM-dd-yyyy");

        try {
            return sdf.parse(date);
        } catch (ParseException e) {
            return null;
        }
    }
}
