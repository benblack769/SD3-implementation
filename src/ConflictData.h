
/*
//Kill operations
//don't worry about partially covered strides/points, flawed, but should cover main use cases
bool contains(Stride, Point)
bool contains(Point, Point)
bool contains(Stride, Stride)

//unit level merging operations
void union_add(unordered_map<Points *> & into, unordered_map<Points *> & outer ){
    if info and outer contain an element, add the accesscount in into of other
    if outer contains an element into does not, add it to into
}
Stride merge_stride(Stride stride1, Stride stride2)
bool can_merge(Stride stride1, Stride stride2)


class ConflictData {

};
This class has 3 basic methods

1. Find conflicts between other ConflictData
2. Merge data from other ConflictData into this one
3. Kill bits in other ConflictData based on this one.

Data structures:

Point:
    PC_ID;
    killed;
    access_size;
    mem_addr;

Stride:
    PC_ID;
    killed;
    access_size;
    mem_start;
    strideLength;

PC_ID:
    int64_t mem_id
    bool MemAccessMode



PC_data:
    vector<Stride *> strides;
    unordered_map<Point *> points;

    bool detecting;
    StrideDetector detector;
    Stride * detector_stride;
    Point * detector_points[2];
    void add_point()
    void add_stride()


unordered_map<PC_ID,PC_data> pc_data;
access_mode_pair<ordered_map<mem_loc,Point*>> points;
access_mode_pair<ordered_map<mem_loc,Stride*>> strides;

void mem_access(PC_ID,memaddr){
    if(points.write[memaddr] is killed (should only be one point at most)))
    else if(find_conflicts(strides.write,map(new_point()))
    else merge_point()
}
void merge_point(Point newpoint){
    pc_data[newpoint.pc].detector;
}
template<IntTy1,IntTy2>
set<IntTy2> killed_adresses(vector<pair<IntTy1,IntTy2>> wrtie_conflicts){
    set<IntTy2> res;
    for <write, new_access> in wrtie_conflicts
        if write.killed and contains(write,new_access)
            res.add(new_access);
}
void merge_pending_history(ConflictData other){
    1. Clear kill bits from points and strides
    1. Get possible read-write conflicts from overlap
    2. Filter out non-conflicts
    3. Report conflicts
    4. Merge in all accesses
}
void merge_history_pending(ConflictData other){
    1. Get possible write conflicts from overlap
    2. Get killed adresses by looking at write conflicts
    3. Filter out killed adresses
    4. Merge in remaining adresses
    //step 6
    for pc_id in other.pc_data.keys()
        if pc_id in pc_data:
            merge_in_pc(pc_data[pc_id],other.pc_data[pc_id])
        else:
            add_new_pc(pc_id,pc_data[pc_id])
}
void merge_in_pc(PC_data my_data,PC_data other_data){
    //add last stride to last stride if only one stride in other_data
    if(other_data.kind == ONE_STRIDE){
        if(can_merge(my_data.only_stride,other_data.only_stride)){
            alter_stride(my_data.last_stride(),merge_stride(my_data.last_stride(),other_data.last_stride()))
        }else  {
            add_stride(other_data.only_stride);
        }
    } else if(other_data.kind == ONE_POINT){
        //similar to one stride
    }
    //add last point to last stride in only ont point in other_data
    if(can_extend(my_data.last_stride(),other_data.last_point())){
        //similar to mergeing strides and strides
    }
    //append all strides
    my_data.strides.extend(other_data.strides)
    //add points to points
    union_add(my_data.points,my_data.points)
}
void remove_stride(Stride * del){
    use stride memloc, accessMode to remove stride from table
}
void remove_point(Point * del){
    use point memloc, accessMode to remove stride from table
}
void add_stride(Stride * new,pc_data mypc_data){
    use stride memloc, accessmode to add stride to table
    mypc_data.add_stride(new);
}
void add_point //similar to add_stride
void alter_stride(Stride * old, Stride new){
    asssert(stride pc is the same);
    remove stride from table
    replace memory at old with new
    place stride onto table again
}
void kill_merge(ConflictData other){
    for pc_id in pc_data.keys()

}

};

*/
#include "dynamic_gcd.h"
#include "IntervalOverlap.h"
#include "Point.h"
#define unordered_map map

template<class IntervalTy>
class PC_Data{
protected:
    //assuming basic "contains" based merge, strides and points
    //that start on the same memory and have the same PC are unique
    unordered_map<int64_t,IntervalTy> intervals;//unordered_map
    typedef typename unordered_map<int64_t,IntervalTy>::iterator map_it_ty;
public:
    void add(IntervalTy interval){
        intervals[interval.first()] = interval;
    }
    void merge_into(PC_Data<IntervalTy> & from){
        //merge if and only if they start at the same mem_address (good for points)
        for(map_it_ty it = from.intervals.begin(); it != from.intervals.end(); it++){
            int64_t key = it->first;
            this->intervals[key] = it->second;
        }
    }
    void append_to(vector<IntervalTy> & cur_data){
        for(map_it_ty it = intervals.begin(); it != intervals.end(); it++){
            cur_data.push_back(it->second);
        }
    }
};

inline Stride merge_strides(Stride one, Stride other){
    assert(one.getPC_ID() == other.getPC_ID());
    return Stride(merge(one,other),one.getPC_ID());
}
class len_offset{
public:
    len_offset(int64_t stride_start,int64_t stride_len):
        len(stride_len),offset(stride_start%stride_len){}
    len_offset():len(-1),offset(-1){}
    bool operator < (len_offset other)const{
        return this->len < other.len ||  (this->len == other.len && this->offset < other.offset);
    }
    bool operator == (len_offset other)const{
        return this->len == other.len &&  this->len == other.len;
    }
protected:
    int64_t len;
    int64_t offset;
};

template<>
class PC_Data<Stride>{
protected:
    typedef map<int64_t,Stride> mem_addr_map;
    typedef unordered_map<len_offset,mem_addr_map> offset_map;
    offset_map intervals;
    typedef typename offset_map::iterator offset_it;
    typedef typename mem_addr_map::iterator mem_addr_it;
public:
    void add(Stride interval){
        len_offset key(interval.first(),interval.stride());
        merge_single_item(intervals[key],interval);
    }
    void merge_into(PC_Data<Stride> & from){
        for(offset_it s_it = from.intervals.begin(); s_it != from.intervals.end(); s_it++){
            len_offset key = s_it->first;
            if(this->intervals.count(key)){
                this->merge_same_stride_len_offset(this->intervals[key], s_it->second);
            }
            else{
                this->intervals[key] = s_it->second;
            }
        }
    }
    void append_to(vector<Stride> & cur_data){
        for(offset_it o_it = intervals.begin(); o_it != intervals.end(); o_it++){
            mem_addr_map & mem_addrs = o_it->second;
            for(mem_addr_it m_it = mem_addrs.begin(); m_it != mem_addrs.end(); m_it++){
                cur_data.push_back(m_it->second);
            }
        }
    }
protected:
    void merge_single_item(mem_addr_map & into, Stride merge_stride){
        int64_t key = merge_stride.first();
        if(into.size() == 0){
            into[key] = merge_stride;
            return;
        }
        
        mem_addr_it greater = into.upper_bound(key);
        
        if(greater != into.begin()){
            mem_addr_it lower_eq_it = greater;
            --lower_eq_it;
            
            Stride lower_eq_stride = lower_eq_it->second;
            //merge in lower or equal vals simply by extending last(), no changes to map
            if(lower_eq_stride.end() >= merge_stride.first()){
                lower_eq_it->second = merge_strides(lower_eq_stride,merge_stride);
            }
        }
        
        for(;greater != into.end() && merge_stride.end() >= greater->second.first(); greater++){
            Stride greater_stride = greater->second;
            //need to change map key (first()), so remove old value from map, then add
            into.erase(greater);
            Stride merged_stride = merge_strides(merge_stride,greater_stride);
            into[merged_stride.first()] = merged_stride;
        }
    }

    void merge_same_stride_len_offset(mem_addr_map & into, mem_addr_map & from){
        if(into.size() == 0){
            into = from;
            return;
        }
        for(mem_addr_it m_it = from.begin(); m_it != from.end(); m_it++){
            Stride merge_stride = m_it->second;
            merge_single_item(into,merge_stride);
        }
    }
};
template<class IntervalTy>
class CompressedData {
protected:
    //typedef  CachedInterval<Point*> point_interval;
    //typedef  CachedInterval<Stride*> stride_interval;
    //typedef multi_set<point_interval, FirstLessThan<point_interval,point_interval> > interval_set;

    //access_mode_pair<multi_set<CachedInterval<Point*> > > points; //ordered_map (maybe shift to unordered)
    //access_mode_pair<multi_set<CachedInterval<Stride*> > > strides; //ordered_map

    typedef typename vector<IntervalTy>::iterator vec_iterator;
    typedef PC_Data<IntervalTy> PC_Data_ty;
    unordered_map<PC_ID,PC_Data_ty> pc_table;//unordered_map
    typedef typename unordered_map<PC_ID,PC_Data_ty>::iterator pc_table_iterator;
public:
    void add(IntervalTy interval){
        pc_table[interval.getPC_ID()].add(interval);
    }
    void merge_into(CompressedData<IntervalTy> & from){
        for(pc_table_iterator it = from.pc_table.begin(); it != from.pc_table.end(); ++it){
            PC_ID id = it->first;
            PC_Data_ty & from_pc_data = it->second;
            this->pc_table[id].merge_into(from_pc_data);
        }
    }
    void clear(){
        pc_table.clear();
    }
    void print(){
        vector<IntervalTy> sorted_vals = sorted_intervals();
        for(vec_iterator it = sorted_vals.begin(); it != sorted_vals.end(); it++){
            cout << *it << endl;
        }
    }

    template<class IntTy1,class IntTy2>
    friend vector<pair<IntTy1,IntTy2> > overlap(CompressedData<IntTy1>,CompressedData<IntTy1>);
protected:
    vector<IntervalTy> sorted_intervals(bool filter = false,MemAccessMode mode = WRITE){
        vector<IntervalTy> intervals;
        for(pc_table_iterator it = pc_table.begin(); it != pc_table.end(); ++it){
            if(!(filter && it->first.get_acc_mode() == mode)){
                PC_Data_ty & pc_data = it->second;
                pc_data.append_to(intervals);
            }
        }
        sort_by_first(intervals);
        return intervals;
    }
};
