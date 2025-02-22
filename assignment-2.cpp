// Program to compute mean, standard deviation, and standard error of book prices.
// Data is read from a file.
// Student ID: [11069686]
// Date: [20/02/2025]

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

// Compute the mean of a dataset.
double compute_mean(double* data, int n)
{
  double sum = 0.0;
  for (int i = 0; i < n; ++i) {
    sum += data[i];
  }
  return sum / n;
}

// Compute the standard deviation of a dataset.
double compute_standard_deviation(double* data, int n)
{
  double mean = compute_mean(data, n);
  double sum = 0.0;
  for (int i = 0; i < n; ++i) {
    sum += (data[i] - mean) * (data[i] - mean);
  }
  return std::sqrt(sum / (n - 1)); // Sample standard deviation
}

// Compute the standard error of the mean.
double compute_standard_error_of_mean(double* data, int n)
{
  double standard_deviation = compute_standard_deviation(data, n);
  return standard_deviation / std::sqrt(n);
}

// Check if a subject is valid (Mathematics, Physics, or Chemistry).
bool is_valid_subject(const std::string& subject)
{
  return (subject == "Mathematics" || subject == "Physics" || subject == "Chemistry");
}

// Check if a subject exists in the books.
bool subject_exists(const std::vector<std::string>& books, const std::string& subject)
{
  for (const auto& book : books) {
    std::stringstream ss(book);
    std::string book_subject;
    ss >> book_subject; // Skip price
    ss >> book_subject; // Get subject
    if (book_subject == subject) {
      return true;
    }
  }
  return false;
}

// Validate y/n input (must be a single character, 'y' or 'n').
bool validate_yes_no(const std::string& input)
{
  if (input.length() != 1) {
    return false; // Input must be a single character
  }
  char ch = tolower(input[0]); // Convert to lowercase
  return (ch == 'y' || ch == 'n');
}

// Main function
int main()
{
  // Define variables
  std::string data_file = "books_data.dat"; // Assuming the file name is fixed
  std::string title;
  std::string author;
  std::string subject;
  std::string bookname;
  double price;

  // Store books in vector (each book as a formatted string)
  std::vector<std::string> books;

  // Open file (you must check if successful)
  std::fstream course_stream(data_file);
  if (!course_stream.is_open()) {
    std::cout << "Error: could not open file " << data_file << std::endl;
    return 1;
  }

  // Read file and store book data
  std::string line;
  while (std::getline(course_stream, line)) {
    std::stringstream ss(line);
    ss >> price;
    ss.ignore(); // Ignore the space after price
    std::getline(ss, subject, ' ');
    std::getline(ss, author, ' ');
    std::getline(ss, bookname);

    // Format price to 2 decimal places without trailing zeros
    std::stringstream price_stream;
    price_stream << std::fixed << std::setprecision(3) << price;
    std::string formatted_price = price_stream.str();

    // Remove trailing zeros and the decimal point if necessary
    formatted_price.erase(formatted_price.find_last_not_of('0') + 1, std::string::npos);
    if (formatted_price.back() == '.') {
      formatted_price.pop_back(); // Remove the decimal point if no fractional part
    }

    // Construct book_info string
    std::string book_info = formatted_price + " " + subject + " " + author + " " + bookname;
    books.push_back(book_info);
  }

  // Close file
  course_stream.close();

  // Total number of records
  int total_records = books.size();
  std::cout << "Total number of records: " << total_records << std::endl;

  // Ask user whether to filter books by subject
  std::string filter_choice;
  while (true) {
    std::cout << "Do you want to filter books by subject? ('y' for further choices, 'n' for all subjects): ";
    std::cin >> filter_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

    if (validate_yes_no(filter_choice)) {
      break; // Exit the loop if input is valid
    } else {
      std::cout << "Invalid input. Please enter 'y' or 'n' (single character only)." << std::endl;
    }
  }

  std::vector<std::string> filtered_books;
  std::string filter_subject;
  if (filter_choice[0] == 'y' || filter_choice[0] == 'Y') {
    while (true) {
      std::cout << "Enter subject to filter by (Enter Mathematics, Physics or Chemistry): ";
      std::getline(std::cin, filter_subject);

      // Check if the subject is valid
      if (!is_valid_subject(filter_subject)) {
        std::cout << "Invalid subject. Please enter Mathematics, Physics, or Chemistry." << std::endl;
        continue;
      }

      // Check if the subject exists in the books
      if (!subject_exists(books, filter_subject)) {
        std::cout << "No books found for subject: " << filter_subject << ". Please try again." << std::endl;
        continue;
      }

      break; // Exit the loop if the subject is valid and exists
    }

    // Filter books based on user's input
    for (const auto& book : books) {
      std::stringstream ss(book);
      ss >> price;
      ss.ignore(); // Ignore the space after price
      ss >> subject;
      if (subject == filter_subject) {
        filtered_books.push_back(book);
      }
    }
  } else {
    // If no filtering, include all books
    filtered_books = books;
  }

  // Number of records for the selection
  int selected_records = filtered_books.size();
  std::cout << "Number of records for the selection: " << selected_records << std::endl;

  // Ask user whether to sort the books by author name
  std::string sort_choice;
  while (true) {
    std::cout << "Do you want to sort the books by author name? (y/n): ";
    std::cin >> sort_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

    if (validate_yes_no(sort_choice)) {
      break; // Exit the loop if input is valid
    } else {
      std::cout << "Invalid input. Please enter 'y' or 'n' (single character only)." << std::endl;
    }
  }

  // Sort books if requested
  if (sort_choice[0] == 'y' || sort_choice[0] == 'Y') {
    std::sort(filtered_books.begin(), filtered_books.end(), [](const std::string& a, const std::string& b) {
      std::stringstream ss_a(a), ss_b(b);
      double price_a, price_b;
      std::string subject_a, author_a, bookname_a;
      std::string subject_b, author_b, bookname_b;
      ss_a >> price_a;
      ss_a.ignore();
      ss_a >> subject_a >> author_a >> bookname_a;
      ss_b >> price_b;
      ss_b.ignore();
      ss_b >> subject_b >> author_b >> bookname_b;
      return author_a < author_b;
    });
  }

  // Compute mean, standard deviation, and standard error of mean for book prices
  std::vector<double> prices;
  for (const auto& book : filtered_books) {
    std::stringstream ss(book);
    ss >> price;
    prices.push_back(price);
  }

  double mean = compute_mean(prices.data(), prices.size());
  double standard_deviation = compute_standard_deviation(prices.data(), prices.size());
  double standard_error_of_mean = compute_standard_error_of_mean(prices.data(), prices.size());

  // Print the filtered and sorted book list
  for (const auto& book : filtered_books) {
    std::cout << book << std::endl;
  }

  // Print computed statistics
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Mean price: " << mean << std::endl;
  std::cout << "Standard deviation: " << standard_deviation << std::endl;
  std::cout << "Standard error of mean: " << standard_error_of_mean << std::endl;

  return 0;
}