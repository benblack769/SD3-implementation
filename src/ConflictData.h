
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
template<class IntervalTy>
class PC_Data{
public:
    //assuming basic "contains" based merge, strides and points
    //that start on the same memory and have the same PC are unique
    map<int64_t,IntervalTy*> intervals;

    typedef typename map<int64_t,IntervalTy*>::iterator map_it;

    bool contains(IntervalTy * interval){
        return intervals.count(interval->first());
    }
    void add(IntervalTy * interval){
        intervals[interval->first()] = interval;
    }
    void remove(IntervalTy * interval){
        intervals.strides.erase(interval->first());
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

    typedef PC_Data<IntervalTy> PC_Data_ty;
    map<PC_ID,PC_Data_ty> pc_table;//unordered_map
    typedef map<PC_ID,PC_Data_ty>::iterator pc_table_iterator;
public:
    void add(IntervalTy * interval){
        pc_table[interval->getPC_ID()].add(interval);
    }
    void set_killed_bits_false(){

    }
    //bool contains(IntervalTy * interval){
    //    return pc_table[interval->getPC_ID()].contains(interval);
    //}
    friend overlap(CompressedData<Point*> ,CompressedData<Point*>);
    friend overlap(CompressedData<Point*> ,CompressedData<Stride*>);
    friend overlap(CompressedData<Stride*> ,CompressedData<Point*>);
    friend overlap(CompressedData<Stride*> ,CompressedData<Stride*>);
protected:
    vector<CachedInterval<IntervalTy> > sorted_intervals(MemAccessMode mode, bool only_killed){
        vector<CachedInterval<IntervalTy> > intervals;
        for(pc_table_iterator it = pc_table.begin(); it != pc_table.end(); ++it){
            if(it->first.accessmode == mode){
                PC_Data & pc_data = it->second;
                for(PC_Data::stride_map_it s_it = pc_data.strides.begin(); s_it != pc_data.strides.end(); ++s_it){
                    IntervalTy * interval = s_it->second;
                    if(!(only_killed && !interval->is_killed())){
                        intervals.push_back(CachedInterval<IntervalTy>(interval));
                    }
                }
            }
        }
        return intervals;
    }
};
typedef CompressedData<Point*> PointTable;
typedef CompressedData<Stride*> StrideTable;
class KillBitsSet{
public:
    set<int64_t> killedbits;
    void set(int64_t addr){
        killedbits.insert(addr);
    }
    bool in(int64_t addr){
        return killedbits.count(addr);
    }
}
class LoopData {
    map<PC_ID,StrideDetector> detectors;
    PointTable pending_points;
    StrideTable pending_strides;

    PointTable history_points;
    StrideTable history_strides;

    int64_t loop_count;

    void addMemAccess(Point point){
        //1. find overlap with killed bits of pending_tables using isKilled
        //2. if some overlap, then return without proceding
        //3. if new pc, add detector
        //4. add point to detetor
        //5. if detector released some points, add them to history tables (handleStrideDetectorResult)
    }
    bool isKilled(Point point){
        //1. construct PointTable with just the single item
        //2. Find overlap between this table and the pendingtables killed bits
        //3. return true if these overlaps have a containment of the point that is killed
    }
    void merge_history_pending(PointTable points, PointTable strides){
        //1. find 4-way overlap between points and strides
        //2. if killed bits don't completely overlap, then add point/stride to pendingtables
    }
    void iteration_end(){
        //1. increment loop_count
        //2. set all kill bits to false in pendingTables
        //3. merge_pending_history
        //4. add conflicts to table
    }
    pair<PointTable, StrideTable> stride_detector_info(){
        //1. go through strides, use purgeStrideDetectorState ideas o get info stored only in stridedetectors
        //2. return that info in new tables
    }
    void conflict_check_pending_history(PointTable points, StrideTable strides){
        //1. create new Point/Stride tables for detectors using stride_detector_info
        //2. find conflicts between them and pending points, strides
        //3. find conflicts between history tables and pending points, strides
        //4. add RAW conflicts to conflict table
    }
    void loop_end(){
        // must be called directly after iteration_end
        //1. flush stridedetectors out to history_tables by merging
    }
};
