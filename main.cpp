#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию
    string type;
    is >> type;

    if(type == "NEW_BUS") {
        q.type = QueryType::NewBus;
        int stops_count = 0;
        is >> q.bus >> stops_count;
        q.stops.resize(stops_count);

        for (string& stop : q.stops){
            is >> stop;
        }

    } else if (type == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (type == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (type == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }

    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    string stop;
    vector<string> buses_for_stop;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if(r.buses_for_stop.empty()) {
        os << "No stop"s;
    } else {
        bool is_first = true;
        for(auto& bus : r.buses_for_stop) {
            if (is_first) {
                os << bus;
                is_first = false;
            } else {
                os << " "s << bus;
            }
        }
    }
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    string bus;
    vector<pair<string, vector<string>>> stops_for_bus;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if(r.stops_for_bus.empty()){
        os << "No bus"s;
    } else  {
        bool first = true;
        for(auto& [stop, buses] : r.stops_for_bus){
            if (!first) {
                os << endl;
            }
            first = false;
            os << "Stop " << stop << ":"s;
            if (buses.size() == 1) {
                os << " no interchange"s;
            } else {
                for (auto& other_bus: buses) {
                    if(other_bus != r.bus) {
                        os << " "s << other_bus;
                    }
                }
            }
        }
    }
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses_with_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {

    if(!r.buses_with_stops.empty()){
        bool is_first = true;
        for(auto [bus, stops] : r.buses_with_stops) {
            if(!is_first) {
                cout << endl;
            }
            is_first = false;
            os << "Bus "s << bus << ":"s;
            for (const string& stop: stops) {
                os << " "s << stop;
            }

        }
    } else {
        os << "No buses"s;
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        for(const string& stop : stops) {
            stops_to_bus_[bus].push_back(stop);
            buses_to_stop_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse r;
        r.stop = stop;
        vector<string> buses_for_stop;

        if(buses_to_stop_.count(stop) != 0){
            for(auto& bus : buses_to_stop_.at(stop)) {
                buses_for_stop.push_back(bus);
            }
        }

        r.buses_for_stop = buses_for_stop;
        return r;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse r;
        r.bus = bus;
        vector<pair<string, vector<string>>> stops_for_bus;

        if(stops_to_bus_.count(bus) != 0) {
            for (auto &stop: stops_to_bus_.at(bus)) {
                pair<string, vector<string>> temp;
                temp.first = stop;
                temp.second = buses_to_stop_.at(stop);
                stops_for_bus.push_back(temp);
            }
        }

        r.stops_for_bus = stops_for_bus;
        return r;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse all_buses;
        all_buses.buses_with_stops = stops_to_bus_;

        return all_buses;
    }
private:
    map<string, vector<string>> buses_to_stop_, stops_to_bus_;
};

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}