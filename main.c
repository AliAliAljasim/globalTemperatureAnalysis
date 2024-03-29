#include <stdio.h>
#define NUM_YEARS 276
#define NUM_MONTHS 12

// Function to calculate yearly averages
// Function to calculate yearly averages
void calculateYearlyAverages(float temperatureData[][NUM_MONTHS],
                             float yearlyAverages[], FILE *file) {
  char line[200]; // Assuming each line in the file is less than 200 characters
  int year, month;
  float temperature;

  // Skip the header line
  fgets(line, sizeof(line), file);

  // Read and process each line in the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Parse the year, month, and temperature; skipping lines without
    // temperature data
    if (sscanf(line, "%d-%d-%*d,%f", &year, &month, &temperature) == 3) {
      if (year >= 1760 &&
          year < 2036) { // Adjusted based on the defined NUM_YEARS
        year -= 1760;
        // Accumulate temperature and count each valid entry to calculate
        // averages later
        temperatureData[year][month - 1] += temperature;
      }
    }
  }

  // Calculate the yearly averages
  for (year = 0; year < NUM_YEARS; year++) {
    float sum = 0;
    int count = 0;
    for (month = 0; month < NUM_MONTHS; month++) {
      if (temperatureData[year][month] != 0) {
        sum += temperatureData[year][month];
        count++;
      }
    }
    // Only calculate average if there is at least one temperature recorded that
    // year
    if (count != 0) {
      yearlyAverages[year] = sum / count;
    }
  }
}

// Function to calculate century averages
// Function to calculate century averages
void calculateCenturyAverages(float temperatureData[][NUM_MONTHS],
                              float centuryAverages[]) {
  float sum[4] = {0}; // Accumulators for 18th, 19th, 20th, and 21st centuries
  int count[4] = {0}; // Counters for the number of data points in each century

  // Iterate over each year and month to accumulate temperature data by century
  for (int year = 0; year < NUM_YEARS; year++) {
    for (int month = 0; month < NUM_MONTHS; month++) {
      int centuryIndex =
          (year + 1760) / 100 -
          17; // Calculate century index (0 for 18th, 1 for 19th, etc.)
      if (centuryIndex >= 0 &&
          centuryIndex < 4) { // Ensure the century index is within bounds
        float temperature = temperatureData[year][month];
        if (temperature != 0) { // Check if there's valid temperature data
          sum[centuryIndex] += temperature;
          count[centuryIndex]++;
        }
      }
    }
  }

  // Calculate the averages for each century
  for (int i = 0; i < 4; i++) {
    if (count[i] > 0) { // Ensure we don't divide by zero
      centuryAverages[i] = sum[i] / count[i];
    } else {
      centuryAverages[i] = 0; // If no data is available, set the average to 0
    }
  }
}

void calculateMonthlyAverages(FILE *file) {
  float monthlySums[NUM_MONTHS] = {0};
  int monthlyCounts[NUM_MONTHS] = {0};
  char line[200]; // Assuming each line in the file is less than 200 characters
  int year, month;
  float temperature;

  rewind(file);

  // Skip the header line
  fgets(line, sizeof(line), file);

  // Read and process each line in the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Parse the year, month, and temperature; ignore lines without temperature
    // data Adjusted sscanf to correctly parse the CSV format
    if (sscanf(line, "%d-%d-%*d,%f", &year, &month, &temperature) == 3) {
      // Check if year is after 1900 and month is within the valid range
      if (year >= 1900 && month >= 1) {
        // Accumulate temperature for the corresponding month and increment the
        // count
        monthlySums[month - 1] += temperature;
        monthlyCounts[month - 1]++;
      }
    }
  }

  // Calculate and print the monthly averages
  for (month = 0; month < NUM_MONTHS; month++) {
    if (monthlyCounts[month] > 0) {
      float monthlyAverage = monthlySums[month] / monthlyCounts[month];
      printf("Average Temperature for Month %d: %.2f\n", month + 1,
             monthlyAverage);
    } else {
      printf("No data available for Month %d\n", month + 1);
    }
  }
}

void findHottestColdestYear(float yearlyAverages[]) {
  // Variables to store the hottest and coldest years and their corresponding
  // temperatures
  int hottestYear = 0, coldestYear = 0;
  float hottestTemp = yearlyAverages[0], coldestTemp = yearlyAverages[0];

  // Find the hottest and coldest years
  for (int year = 1; year < NUM_YEARS; year++) {
    if (yearlyAverages[year] > hottestTemp) {
      hottestYear = year;
      hottestTemp = yearlyAverages[year];
    }
    if (yearlyAverages[year] < coldestTemp && yearlyAverages[year] != 0) {
      coldestYear = year;
      coldestTemp = yearlyAverages[year];
    }
  }

  // Print the hottest and coldest years along with their average temperatures
  printf("Hottest Year: %d, Average Temperature: %.2f\n", 1760 + hottestYear,
         hottestTemp);
  printf("Coldest Year: %d, Average Temperature: %.2f\n", 1760 + coldestYear,
         coldestTemp);
}

void findHottestColdestMonth(FILE *file) {
    char line[200];  // Assuming each line in the file is less than 200 characters
    char hottestMonth[10], coldestMonth[10];  // Changed the size to 10 to store only the month and year
    float hottestTemp = -9999.0, coldestTemp = 9999.0;
    float temp;
    int year, month;

    rewind(file);
    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read and process each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Parse the date, land average temperature, and other data if needed
        if (sscanf(line, "%d-%d-%*d,%f", &year, &month, &temp) == 3) {
          
            // Check if the current temperature is hotter than the hottest recorded
            if (temp > hottestTemp) {
                hottestTemp = temp;
                sprintf(hottestMonth, "%d-%02d", year, month);  // Store the hottest month and year
            }
            // Check if the current temperature is colder than the coldest recorded
            if (temp < coldestTemp) {
                coldestTemp = temp;
                sprintf(coldestMonth, "%d-%02d", year, month);  // Store the coldest month and year
            }
        }
    }

    // Print the hottest and coldest months recorded
    printf("Hottest Month Recorded: %s, Temperature: %.2f\n", hottestMonth, hottestTemp);
    printf("Coldest Month Recorded: %s, Temperature: %.2f\n", coldestMonth, coldestTemp);
}

int main() {
  FILE *file;
  float temperatureData[NUM_YEARS][NUM_MONTHS] = {
      0};                                // Initialize all elements to 0
  float yearlyAverages[NUM_YEARS] = {0}; // Initialize all elements to 0
  float centuryAverages[4] = {0};        // Initialize all elements to 0

  // Open the CSV file for reading
  file = fopen("GlobalTemperatures.csv", "r");
  if (file == NULL) {
    perror("Error opening the file");
    return 1;
  }

  // Calculate the yearly averages
  printf("Question 1:\n");
  calculateYearlyAverages(temperatureData, yearlyAverages, file);
  // Calculate the century averages
  calculateCenturyAverages(temperatureData, centuryAverages);

  // Print the yearly averages for each year
  for (int year = 1760; year < 2026; year++) {
    if (yearlyAverages[year - 1760] != 0) {
      printf("In the Year %d the average temperature is %.2f\n", year,
             yearlyAverages[year - 1760]);
    }
  }
  printf("\n");
  // Print the century averages
  printf("Question 2:\n");
  printf("18th Century Average: %.2f\n", centuryAverages[0]);
  printf("19th Century Average: %.2f\n", centuryAverages[1]);
  printf("20th Century Average: %.2f\n", centuryAverages[2]);
  printf("21st Century Average: %.2f\n\n", centuryAverages[3]);

  // Calculate and print the monthly averages
  printf("Question 3:\n"); 
  calculateMonthlyAverages(file);
  printf("\n");

  // Calculate the Hottest and coldest Monthes
  printf("Question 4:\n");
  findHottestColdestMonth(file);
  printf("\n");

  
  // Find and print the hottest and coldest year
  printf("Question 5:\n");
  findHottestColdestYear(yearlyAverages);
  // Close the file
  fclose(file);

  return 0;
}
