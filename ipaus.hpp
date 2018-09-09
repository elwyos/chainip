#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;
using namespace std;
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/*

   Note about this smart contract : it's incomplete
   Mostly coded for illustrative purposes.
   Code is based on my previous hackathon, and 
   smart contracts generally work similarly to this.
   
   For more info, feel free to mail yose@jormy.com

*/
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#include "hackyhelpers.hpp"
// hackathon helper function library, up there.
// the above function helpers are there to make typing tables less error-prone
// and makes things much easier to read
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
class ipaus : public eosio::contract{
   public:
      using contract::contract;
      // the lines above are standard for a contract. standard boilerplate, really
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     
      // customer represents either applicant or rights owners.
      // of course this depends on the actual rules of the organization
      // these are for illustrative purposes mostly
     
      //@abi table customers
      class customer{
         public:
            account_name account; 
               // a 64 bit number, which translates to a 12 character username (a-z lowercase only)
               // a standard 
            string name; // e.g koala
            int32_t type; // enum -- 0 for SME, 1 for large corporation
            
            // all the different old identifiers
            string party_id;
            string sap_customer_id;
            string ccs_parms_customer_id;
            string ipa_customer_id; // IPA customer ID (non-blockchain)
            string abn; // australian business number
            string acn; // australian company number
            
            void init(); // this is a method which initializes the data on a new record
            
            int64_t primary_key() const { return account; }; // the blockchain username of said customer
      };
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      // intellectual property class - describes entries in the intellectual property table
      // EOS table names are limited to 12 chars, because they just happen to fit into a 64 bit number
      
      //@abi table intelprops
      class intelprop{
         public:
         
            int64_t intelprop_id;          
         
            // data dictionary, taken from the table description
            string appli_num;
            int32_t appli_year;
            account_name applicant; // reference to the Customer record from 
            account_name owner; // reference to the Customer record, but as the applicant role
            
            checksum256 document_hash;
            
            int64_t merge_target;
            bool merged;
            
            void init();
            
            int64_t primary_key() const { return animalid; };
            uint64_t getapplicant() const { return applicant; }; // no underscores in indexes
            uint64_t getowner() const { return owner; };
            
      };
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      // a record of the merger
      
      //@abi table mergerecs
      class mergerec{
         public:
            account_name approver; // which employee approved the merger?
            
            vector<int64_t> intelprop_ids; // records to merge
            
            uint64_t file_time; // time which this merge was filed
            
      };
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      // employee represents someone who may or may not have the ability to
      // approve/reject applications. Included here just for illustrative purposes
      
      //@abi table employees
      class employee{
         public:
            account_name name;
            string fullname; // full name of the employee
            bool employed; // currently employed or not?
            
            int64_t primary_key() const { return name; };
            
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      // metadata table entry - only one entry in the table and is easily accessible
      // using the singleton pattern for this table
      
      //@abi table metadata
      class metadata{
         public:
            int64_t ip_idcounter;
            // for creating new records
            
            void init();
            
            int64_t primary_key() const{ return 0;  };
            
      };
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // table declarations
      // in EOS, when accessing tables, you have to create multi index objects.
      // creating these multi index objects the usual way is clunky, so we're using
      // helpers which make the process more elegant
      
      //@abi table
      shingleton(metadata) METADATA;
      tablemapp(customers, account) CUSTOMER;
      tablemapp(employees, employee) EMPLOYEE;
      tablemapp2(intelprops, intelprop, applicant, getapplicant, owner, getowner) INTELPROP;
         // you can add additional indexes to tables, and the way you do it with multi indexes
         // are by specifying indexers in the template.
         // however, they are prone to errors and are complicated to type and remember, which is why
         // we're using a helper macro tablemapp
      tablemapp(mergerecs, mergerec) MERGEREC;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // smart contract methods  / functions
      // this is where the fun begins
      // some of these will be publicly callable, and some of these would be only helper functions
      // which ones are available to be called on the blockchain needs to be declared in ipaus.cpp using the EOSIO_ABI macro
      
      void createcust(account_name accname, const string& name, const string& ipa_id);
      
      void fileip(const string& ipa_filing_id, checksum256 hash);
      
      void approveip(account_name approver, int64_t ip_id);
      void rejectip(account_name approver, int64_t ip_id);
      
      void mergeip(account_name approver, int64_t ip_one, int64_t ip_two);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
