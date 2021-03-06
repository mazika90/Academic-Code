#include <sstream>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

class Airplane {
	int id, bef, target, last, landing_time, runway, min_landing_time, current_left, current_right;
	float p_bef, p_last;
	vector<int> sep;
	map<int,bool> domain_blacklist;
	bool left_blocked = false, right_blocked = false;
	
	public:
		// setters
		void set_values(int id, int bef, int target, int last, float p_bef, float p_last){
			this->id = id;
			this->bef = bef;
			this->target = target;
			this->last = last;
			this->p_bef = p_bef;
			this->p_last = p_last;
			landing_time = -1;
			runway = -1;
			min_landing_time = bef;
			current_left = target;
			current_right = target;
		}
		void push_sep(int value){
			this->sep.push_back(value);
		}
		void set_landing_time(int value){
			this->landing_time = value;
			this->min_landing_time = value;
		}
		void reset_min_landing(){
			this->min_landing_time = this->bef;
		}
		void set_runway(int value){
			this->runway = value;
		}
		int increase_min_landing_time(){
			return this->min_landing_time >= this->last ? this->last : this->min_landing_time++;
		}
		void reset_domain(){
			current_left = target;
			current_right = target;
			left_blocked = false;
			right_blocked = false;
		}

		// getters
		int get_id() {return id;}
		int get_bef() {return bef;}
		int get_target() {return target;}
		int get_last() {return last;}
		float get_p_bef() {return p_bef;}
		float get_p_last() {return p_last;}
		int get_sep(int i) {return sep[i];}
		int get_min_landing_time() {return min_landing_time;}


		// misc functions
		bool verify_time(int suggested_time){
			if(suggested_time >= this->bef && suggested_time <= this->last)
				return true;
			else
				return false;
		}
		bool check_collision(int suggested_time, int plane_id){
			if(suggested_time >= this->landing_time && suggested_time <= this->landing_time + sep[plane_id] && plane_id != this->id)
				return true;
			else
				return false;
		}
		float get_cost(int suggested_time){
			if(!this->verify_time(suggested_time))
				return 99999;
			else if(suggested_time >= this->bef && suggested_time < this->target)
				return (float) (this->target - suggested_time)*this->p_bef;
			else if(suggested_time > this->target && suggested_time <= this->last)
				return (float) (suggested_time - this->target)*this->p_last;
			else
				return 0;

		}

		int get_best_landing_time(){
			if(this->current_left==this->target && this->current_right==this->target){
				this->current_left = this->target - 1;
				this->current_right = this->target + 1;
				return target;
			}

			if (right_blocked && left_blocked)
				return -1;

			
			if (this->current_left == this->bef && !this->left_blocked){
				float cost_left = (this->target - this->current_left)*p_bef;
				float cost_right = (this->current_right - this->target)*p_last;

				if (cost_left > cost_right){
					int temp_value = this->current_right;
					this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
					return temp_value;
				}
				else if (cost_right > cost_left){
					this->left_blocked = true;
					int temp_value = this->current_left;
					this->current_left = this->current_left - 1 < this->bef ? this->bef : this->current_left - 1;
					return temp_value;
				}
				else{
					this->left_blocked = true;
					int temp_value = this->current_left;
					this->current_left = this->current_left - 1 < this->bef ? this->bef : this->current_left - 1;
					return temp_value;
				}
			}
			else if(this->left_blocked){
				int temp_value = this->current_right;
				this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
				return temp_value;
			}
			if (this->current_right == this->last && !this->right_blocked){
				float cost_left = (this->target - this->current_left)*p_bef;
				float cost_right = (this->current_right - this->target)*p_last;

				if (cost_left > cost_right){
					this->right_blocked = true;
					int temp_value = this->current_right;
					this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
					return temp_value;
				}
				else if (cost_right > cost_left){
					int temp_value = this->current_left;
					this->current_left = this->current_left - 1 < this->bef ? this->bef : this->current_left - 1;
					return temp_value;
				}
				else{
					this->right_blocked = true;
					int temp_value = this->current_right;
					this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
					return temp_value;
				}
			}
			else if(this->right_blocked){
				int temp_value = this->current_left;
				this->current_left = this->current_left - 1 < this->bef ? this->bef : this->current_left - 1;
				return temp_value;
			}


			float cost_left = (this->target - this->current_left)*p_bef;
			float cost_right = (this->current_right - this->target)*p_last;

			if (cost_left > cost_right){
				int temp_value = this->current_right;
				this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
				return temp_value;
			}
			else if (cost_right > cost_left){
				int temp_value = this->current_left;
				this->current_left = this->current_left - 1 < this->bef ? this->bef : this->current_left - 1;
				return temp_value;
			}
			else{
				int temp_value = this->current_right;
				this->current_right = this->current_right + 1 > this->last ? this->last : this->current_right + 1;
				return temp_value;
			}

			
		}
};

// fill a vector with the airplane data from a file
vector<Airplane> populate(char* filename){
    int plane_count, i,j;
	string line, sp_temp;
	string temp_str[5];
	Airplane temp;
	vector<Airplane> airplanes; // this is the return variable

	/* Get plane count from the first line of the file */
	ifstream input(filename);
	getline(input, line);
	plane_count = atoi(line.c_str());
	
	/* Safely populate a list of Airplanes */
	for(i=0 ; i < plane_count ; i++){
		
		/* Get airplane i data */
		getline(input, line);
    	stringstream ap(line);
    	// the following 'while' splits a string separated by spaces into a fixed array
    	j = 0;
		while (ap.good() && j < 5){
			ap >> temp_str[j];
			++j;
		}
		// populate temp airplane data
		temp.set_values(i, atoi(temp_str[0].c_str()), atoi(temp_str[1].c_str()), atoi(temp_str[2].c_str()), atof(temp_str[3].c_str()), atof(temp_str[4].c_str()));
			
		
		/* Get airplane separation time for airplane i */
		getline(input, line);
    	stringstream sp(line);
    	// the following 'while' splits a string separated by spaces into a fixed array
    	j = 0;
		while (sp.good() && j < plane_count){
			sp >> sp_temp;
			temp.push_sep(atoi(sp_temp.c_str()));
			++j;
		}
		
		airplanes.push_back(temp);
		//cout << "\rLoading airplanes data from file... " << (((i+1)*100)/plane_count) << "%";
	}

	//cout << endl;
	return airplanes;
}

// sorting functions
vector<Airplane> sortByTarget(vector<Airplane> airplanes){
	Airplane airplane;
	for(int i=0 ; i<airplanes.size() ; i++){
		for(int j=0 ; j<i ; j++){
			if(airplanes.at(i).get_target() < airplanes.at(j).get_target()){
				swap(airplanes.at(i), airplanes.at(j));
			}
		}
	}
	return airplanes;
}
vector<Airplane> sortByBef(vector<Airplane> airplanes){
	Airplane airplane;
	for(int i=0 ; i<airplanes.size() ; i++){
		for(int j=0 ; j<i ; j++){
			if(airplanes.at(i).get_bef() < airplanes.at(j).get_bef()){
				swap(airplanes.at(i), airplanes.at(j));
			}
		}
	}
	return airplanes;
}
vector<Airplane> sortByLast(vector<Airplane> airplanes){
	Airplane airplane;
	for(int i=0 ; i<airplanes.size() ; i++){
		for(int j=0 ; j<i ; j++){
			if(airplanes.at(i).get_last() < airplanes.at(j).get_last()){
				swap(airplanes.at(i), airplanes.at(j));
			}
		}
	}
	return airplanes;
}

// checker functions
bool checkPossibleCollision(Airplane a, Airplane b){
	if(a.get_last() + a.get_sep(b.get_id()) >= b.get_bef() && b.get_last() >= a.get_bef())
		return true;
	return false;
}