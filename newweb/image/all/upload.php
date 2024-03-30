<?php
$target_dir = "/Users/ahajji/Desktop/newweb/image/all/";  // Specify your target directory here

// if ($_SERVER['REQUEST_METHOD'] == 'POST') {
//     // Check if file was uploaded without errors
//     if (isset($_FILES["myfile"]) && $_FILES["myfile"]["error"] == 0) {
//         $target_file = $target_dir . basename($_FILES["myfile"]["name"]);

//         // Check if file already exists
//         if (file_exists($target_file)) {
//             echo "Sorry, file already exists.";
//         } else {
//             // Attempt to move the uploaded file to your desired directory
//             if (move_uploaded_file($_FILES["myfile"]["tmp_name"], $target_file)) {
//                 echo "The file ". basename($_FILES["myfile"]["name"]). " has been uploaded.";
//             } else {
//                 echo "Sorry, there was an error uploading your file.";
//             }
//         }
//     } else {
//         echo "No file was uploaded.";
//     }
// }
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get the text data from the POST request
    $textData = "hi i am karim";

    // Define the file path where the data will be stored
    $filePath = $target_dir.'data.txt';

    // Open the file in write mode
    $file = fopen($filePath, 'w');

    // Write the text data to the file
    fwrite($file, $textData);

    // Close the file
    fclose($file);

    echo "Data saved to file successfully.";
} else {
    // If data was not sent via POST method, show an error message
    echo "Error: Data not received via POST method.";
}
?>