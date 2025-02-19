#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <curl/curl.h>
    
    // Twilio Credentials
    #define TWILIO_SID "your_twilio_sid"
    #define TWILIO_AUTH_TOKEN "your_twilio_auth_token"
    #define TWILIO_PHONE_NUMBER "+your_twilio_phone_number"
    #define RECIPIENT_PHONE_NUMBER "+recipient_phone_number"
    
    // Email Credentials
    #define SMTP_SERVER "smtp.gmail.com"
    #define SMTP_PORT 587
    #define EMAIL_USERNAME "your_email@gmail.com"
    #define EMAIL_PASSWORD "your_email_password"
    #define RECIPIENT_EMAIL "recipient_email@example.com"
    
    // Function to send an SMS alert using Twilio
    void send_sms_alert(const char *message) {
        CURL *curl;
        CURLcode res;
        char url[256];
        char post_fields[512];
    
        snprintf(url, sizeof(url), "https://api.twilio.com/2010-04-01/Accounts/%s/Messages.json", TWILIO_SID);
        snprintf(post_fields, sizeof(post_fields),
                 "To=%s&From=%s&Body=%s",
                 RECIPIENT_PHONE_NUMBER, TWILIO_PHONE_NUMBER, message);
    
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
            curl_easy_setopt(curl, CURLOPT_USERNAME, TWILIO_SID);
            curl_easy_setopt(curl, CURLOPT_PASSWORD, TWILIO_AUTH_TOKEN);
    
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                fprintf(stderr, "SMS send failed: %s\n", curl_easy_strerror(res));
            else
                printf("SMS Alert Sent Successfully!\n");
    
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
    
    // Function to send an email alert (uses system command)
    void send_email_alert(const char *message) {
        char command[512];
        snprintf(command, sizeof(command),
                 "echo \"%s\" | mail -s \"Disaster Alert\" -r %s -S smtp=smtp://%s:%d -S smtp-auth=login -S smtp-auth-user=%s -S smtp-auth-password=%s %s",
                 message, EMAIL_USERNAME, SMTP_SERVER, SMTP_PORT, EMAIL_USERNAME, EMAIL_PASSWORD, RECIPIENT_EMAIL);
    
        int status = system(command);
        if (status == 0)
            printf("Email Alert Sent Successfully!\n");
        else
            printf("Failed to send Email Alert!\n");
    }
    
    // Function to read disaster data (simulate API response)
    void check_disaster_alert() {
        FILE *file = fopen("disaster_data.txt", "r"); // Simulated data source
        if (file == NULL) {
            printf("Error reading disaster data!\n");
            return;
        }
    
        char type[50], location[50];
        int severity;
        while (fscanf(file, "%s %s %d", type, location, &severity) != EOF) {
            if (severity >= 7) {  // Trigger alert if severity is high
                char alert_message[256];
                snprintf(alert_message, sizeof(alert_message),
                         "🚨 Alert: %s detected in %s with severity %d. Take immediate action!",
                         type, location, severity);
    
                send_sms_alert(alert_message);
                send_email_alert(alert_message);
            }
        }
    
        fclose(file);
    }
    
    // Main function
    int main() {
        printf("Checking for disaster alerts...\n");
        check_disaster_alert();
        return 0;
    }