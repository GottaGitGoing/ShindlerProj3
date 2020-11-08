#include "Wordset.hpp"
#include <string>
#include <memory>
#include <iostream>

namespace
{
	const unsigned int INIT_CAP = 11;
	const int BASE_TO_USE = 41; 
	const double LOAD_LIMIT = 0.27;

}

// returns s, as a number in the given base, mod the given modulus
int hashFunction(std::string s, int base, int mod)
{
	// return 0; // this is not correct.
	int hashed = 0;
	for (unsigned int i = 0; i< s.size(); ++i)
	{
		hashed = (base*hashed + (s[i]- 'a')) % mod;
	}
	
	return hashed;
}


WordSet::WordSet()
	:capacity{INIT_CAP}, sz{0}, buckets{new std::string[INIT_CAP]}
{

}

WordSet::~WordSet()
{

}

void WordSet::insert(std::string s)
{
	
	int placement = hashFunction(s,BASE_TO_USE,getCapacity());
	// if no collision, simply place item in bucket
	double hash_load = double(getCount()+1)/double(getCapacity());
	if (hash_load < LOAD_LIMIT)
	{
		if (this->buckets[placement] == "")
		{
			buckets[placement] = s;
			++sz;
		}
		else
		{
			// there is collision. Resolution needed
			std::cout << s << " collision with " << this->buckets[placement] << std::endl;
			int resolved_placement = findEmptyBucket(placement,this->buckets,getCapacity());
			buckets[resolved_placement] = s;
			++sz;
		}
	}
	else // The need to rehash.
	{
		std::cout << " REHASH " << std::endl;
		unsigned int new_cap = (getCapacity()*2) +1;
		// std::cout << new_cap << " -- " << std::endl;
		std::unique_ptr<std::string[]> new_buckets = std::make_unique<std::string[]>(new_cap);
		// rehash the items currently in the old bucket
		for (unsigned int i=0; i<getCapacity(); ++i)
		{
			if (this->buckets[i] == "") // if old bucket is empty, check the next one
			{
				continue;
			}
			// here you hit a non-empty string of old bucket. It'll rehash			
			int rehash_place = hashFunction(this->buckets[i],BASE_TO_USE,new_cap);
			if (new_buckets[rehash_place] == "")
			{
				// if new bucket doesnt have collision. Go ahead
				new_buckets[rehash_place] = this->buckets[i];
			}
			else
			{	
				// collison! So probe!!
				int resolved_rehash = findEmptyBucket(rehash_place, new_buckets, new_cap);
				new_buckets[resolved_rehash] = this->buckets[i];
				
			}	
		}
		
		// std::cout << getCapacity() << " -----" <<std::endl;
		// for (unsigned int f=0;f<getCapacity();++f)
		// {
		// 	std::cout << "Curr buck " << this->buckets[f] << std::endl;
		// }
		this->buckets = std::move(new_buckets);

		this->capacity = new_cap;
	}

	
	
	
	
}

int WordSet::findEmptyBucket(int & full_bucket, std::unique_ptr<std::string[]>& the_bucket, int the_cap )
{
	// to_return = hash s + i**2 mod it
	// if the bucket is empty return int
	int buck_to_check = 0;
	unsigned int i = 1;
	while (true)
	{
		buck_to_check = (full_bucket + (i*i)) % the_cap;
		if (the_bucket[buck_to_check] == "") // if reached empty, then resolved
		{
			return buck_to_check;
		}
		else
		{
			++i;
		}
		
	}
	

}

bool WordSet::contains(std::string s) const
{
	int placement = hashFunction(s, BASE_TO_USE, getCapacity());
	unsigned int count = 1;
	unsigned int temp_buck = placement;
	while (this->buckets[temp_buck] != "")
	{
		if (this->buckets[temp_buck] == s) return true;
		else
		{
			temp_buck = (placement + (count*count)) % getCapacity();
			++count;
		}
	}
	return false;	
}

// return how many distinct strings are in the set
int WordSet::getCount() const
{
	return sz;
	// return BASE_TO_USE + LOAD_LIMIT; // this is not the right answer. 
}

// return how large the underlying array is.
int WordSet::getCapacity() const
{
	for (unsigned int i =0;i<capacity;++i)
	{
		std:: cout << i << this->buckets[i] << " --" << std::endl;
	}
	return capacity;
	// return 1729; // that isn't even a prime number!
}
