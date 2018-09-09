////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// hacky helpers, really generic stuff
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
struct checksum64x4{
   uint64_t one;
   uint64_t two;
   uint64_t three;
   uint64_t four;
   
   checksum64x4(){
      one = 0;
      two = 0;
      three = 0;
      four = 0;
   }
   
   checksum64x4(const checksum256& lesix){
      static_assert(sizeof(checksum256) == sizeof(checksum64x4), "wut, sizes are different");
      *this = (*(checksum64x4*)&lesix);
   }
   
   checksum64x4 operator^(const checksum64x4& rhs){
      
      checksum64x4 ret;
      ret.one = one ^ rhs.one;
      ret.two = two ^ rhs.two;
      ret.three = three ^ rhs.three;
      ret.four = four ^ rhs.four;
      return ret;
   }
   
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
string stringify(account_name lename){
   return eosio::name{lename}.to_string();
}
////////////////////////////////////////////////////////////////////////////
char num_to_hex_char(int lenum){
   if(lenum >= 0 && lenum < 10)
      return '0' + lenum;
     
   return 'a' + (lenum - 10);
}
////////////////////////////////////////////////////////////////////////////
string stringify(checksum256 lesum){
     
   
   string ret;
   
   int i;
   for(i = 0; i < 32; i++){
      uint8_t thebyte = lesum.hash[i];
      
      int firstnum = thebyte >> 4;
      int secondnum = thebyte & 0xf;
      
      ret.append(1, num_to_hex_char(firstnum));
      ret.append(1, num_to_hex_char(secondnum));
      
   }
   
   return ret;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
template <int linenueesm>
void __shingassert(const string& msg, const char* filename, int linenum, const char * expression){
   string totalmsg = "shingassert:\n" + msg + "\n@" + string(filename) + ":" + std::to_string(linenum) + "\n[" + string(expression) + "]";
   eosio_assert(false, totalmsg.c_str());
}
////////////////////////////////////////////////////////////////////////////
#define x_assert(e, msg) if(!(e)) __shingassert<0>(msg, __FILE__, __LINE__, #e);
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// hacky singleton class for super convenience
////////////////////////////////////////////////////////////////////////////
template <uint64_t indexname, class Whatever>
class shingletonney{
   public:
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Whatever val;
      bool ondb;
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      multi_index<indexname, Whatever> _t;
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      shingletonney(uint64_t code):
         _t(code, N(singleton)){
            // load straight away
            load();
         }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      void load(){
         auto it =_t.find(0);
         
         if(it == _t.end()){
            // no need to emplace, because it's just an inited thing
            val.init();
            ondb = false;
         } else {
            val = *it;
            ondb = true;
         
         }
      }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Whatever * operator->(){
         return &val;
      }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      void clear(){
         val = Whatever();
         val.init();
         
         auto it = _t.find(0);
         if(it != _t.end())
            _t.erase(it);
            
         ondb = false;
      }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      void save(account_name payer){
         auto it = _t.find(0);
         if(it == _t.end()){
            _t.emplace(payer, [this](Whatever& w){
                  w = val;
               });
         } else {
            _t.modify(it, payer, [this](Whatever& w){
                  w = val;
               });
         }
         ondb = true;
      }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// hacky template subclass of multi_index for even more convenience
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
template <uint64_t Tablename, class Whatever, class... Indices>
class shingmapler : public multi_index<Tablename, Whatever, Indices...>{
   public:
      shingmapler(uint64_t code):
         multi_index<Tablename, Whatever, Indices...>(code, code){
            // nothing yet
         }
      shingmapler(uint64_t code, uint64_t lescope):
         multi_index<Tablename, Whatever, Indices...>(code, lescope){
            // nothing yet
         }
      
      // hmm, we should add as we go?
      
      //template <uint64_t theindex>
      //auto by(){
         //return this->get_index<theindex>();
      //}
      
      template <uint64_t theindex>
      void filterdo(uint64_t thevalue, std::function<void(const Whatever&)> lefunc){
         auto indexer = this->template get_index<theindex> ();
         auto it = indexer.lower_bound(thevalue);
         auto enda = indexer.upper_bound(thevalue);
         while(it != enda){
            auto tempit = it;
            it++;
            // can potentially delete it, so advance it first
            lefunc(*tempit);
         }
      }
      
      void emptify(){
         while(true){
            auto it = this->begin();
            if(it == this->end()){
               break;
            } else {
               this->erase(it);
            }
             
         }
      }
      
      bool has(uint64_t primkey){
         auto it = this->find(primkey);
         if(it == this->end())
            return false;
         return true; // gots it!
      }
      
      Whatever mustfetch(uint64_t primkey){
         Whatever ret;
         bool succ = fetch(primkey, ret);
         x_assert(succ, "No such entry");
         return ret;
      }
      
      bool fetch(uint64_t primkey, Whatever& thewhatever){
         auto it = this->find(primkey);
         if(it != this->end()){
            thewhatever = *it;
            return true;
         } else {
            thewhatever.init(); // init it regardless
            return false;
         }
      }
      
      void remove(uint64_t primkey){
         auto it = this->find(primkey);
         if(it != this->end())
            this->erase(it);
      }
      
      void save(account_name payer, const Whatever& thewhatever){
         auto it = this->find(thewhatever.primary_key());
         if(it == this->end()){
            x_assert(payer != 0, "Trying to emplace, but no payer");
            this->emplace(payer, [thewhatever](Whatever& w){
                  w = thewhatever;
               });
         } else {
            this->modify(it, payer, [thewhatever](Whatever& w){
                  w = thewhatever;
               });
         }
      }
      
      void modemplace(int64_t primkey, account_name payer, std::function<void(Whatever&)> funcer){
         auto it = this->find(primkey);
         if(it == this->end()){
            x_assert(payer != 0, "Trying to emplace, but no payer");
            this->emplace(payer, [funcer](Whatever& w){
                  funcer(w);
               });
         } else {
            this->modify(it, payer, [funcer](Whatever& w){
                  funcer(w);
               });
         }
      }
      
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#define tablemapp(tablename, classname) \
   typedef shingmapler<N(tablename), classname>
////////////////////////////////////////////////////////////////////////////
#define tablemapp1(tablename, classname, indexname1, funcname1) \
   typedef shingmapler<N(tablename), classname \
                        , indexed_by<N(indexname1), const_mem_fun<classname, uint64_t, &classname::funcname1> > \
                        >
////////////////////////////////////////////////////////////////////////////
#define tablemapp2(tablename, classname, indexname1, funcname1, indexname2, funcname2) \
   typedef shingmapler<N(tablename), classname \
                        , indexed_by<N(indexname1), const_mem_fun<classname, uint64_t, &classname::funcname1> > \
                        , indexed_by<N(indexname2), const_mem_fun<classname, uint64_t, &classname::funcname2> > \
                        >
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#define shingleton(classname) \
   typedef shingletonney<N(classname), classname> 
////////////////////////////////////////////////////////////////////////////
#define shingleton_as(classname, used_name) \
   typedef shingletonney<N(used_name), classname> 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
