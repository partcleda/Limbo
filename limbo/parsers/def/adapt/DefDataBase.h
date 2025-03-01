/**
 * @file   DefDataBase.h
 * @brief  Database for Def parser 
 * @author Yibo Lin
 * @date   Oct 2014
 */

#ifndef DEFPARSER_DATABASE_H
#define DEFPARSER_DATABASE_H

#include "limbo/thirdparty/lefdef/5.8/def/def/defiNet.hpp"
#include "limbo/thirdparty/lefdef/5.8/def/def/defiRowTrack.hpp"
#include "limbo/thirdparty/lefdef/5.8/def/def/defiVia.hpp"
#include <array>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <limbo/thirdparty/lefdef/5.8/def/def/defrReader.hpp>
#include <limbo/thirdparty/lefdef/5.8/def/def/defiAlias.hpp>

/// namespace for DefParser
namespace DefParser {

/// @nowarn
using std::cout;
using std::endl;
using std::cerr;
using std::string; 
using std::vector;
using std::pair;
using std::make_pair;
using std::ostringstream;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
/// @endnowarn

/// @brief Temporary data structures to hold parsed data. 
/// Base class for all temporary data structures. 
struct Item 
{
    /// print data members 
	virtual void print(ostringstream&) const {};
    /// print data members with stream operator 
    /// @param os output stream 
    /// @param rhs target object 
    /// @return output stream 
	friend std::ostream& operator<<(std::ostream& os, Item const& rhs)
	{
		std::ostringstream ss;
		rhs.print(ss);
		os << ss.str();
		return os;
	}
    /// print data members with stream operator 
    /// @param ss output stream 
    /// @param rhs target object 
    /// @return output stream 
	friend ostringstream& operator<<(ostringstream& ss, Item const& rhs)
	{
		rhs.print(ss);
		return ss;
	}
};
/// @brief placement row 
struct Row : public Item
{
	string row_name; ///< row name 
	string macro_name; ///< macro name of row 
	int32_t origin[2]; ///< x, y of origin 
	string orient; ///< orientation 
	int32_t repeat[2]; ///< do x by y  
	int32_t step[2]; ///< x, y
    /// @brief reset all data members 
	void reset()
	{
		row_name = macro_name = orient = "";
		origin[0] = origin[1]  = -1;
		repeat[0] = repeat[1] = -1;
		step[0] = step[1] = -1;
	}
    /// @brief print data members 
    /// @param ss output stream 
	virtual void print(ostringstream& ss) const
	{
		ss << "//////// Row ////////" << endl
			<< "row_name = " << row_name << endl 
			<< "macro_name = " << macro_name << endl 
			<< "origin = " << origin[0] << " " << origin[1] << endl 
			<< "orient = " << orient << endl 
			<< "repeat = " << repeat[0] << " " << repeat[1] << endl 
			<< "step = " << step[0] << " " << step[1] << endl; 
	}
};
/// @brief cell in placement 
struct Component : public Item
{
	string comp_name; ///< component name 
	string macro_name; ///< macro name of component, standard cell type 
	string status; ///< placement status 
	int32_t origin[2]; ///< x, y of origin 
	string orient; ///< orientation 
    /// @brief reset all data members 
	void reset()
	{
		comp_name = macro_name = status = orient = "";
		origin[0] = origin[1] = -1;
	}
    /// @brief print data members 
    /// @param ss output stream 
	virtual void print(ostringstream& ss) const
	{
		ss << "//////// Component ////////" << endl
			<< "comp_name = " << comp_name << endl 
			<< "macro_name = " << macro_name << endl 
			<< "status = " << status << endl 
			<< "origin = " << origin[0] << " " << origin[1] << endl 
			<< "orient = " << orient << endl; 
	}
};
/// @brief port of pin 
struct PinPort : public Item
{
	string status; ///< placement status 
	int32_t origin[2]; ///< offset to node origin 
	string orient; ///< orientation 
    vector<string> vLayer; ///< layers  
    vector<vector<int32_t> > vBbox; ///< bounding box on each layer 
    /// @brief reset all data members 
	void reset()
	{
		status = orient = "";
		origin[0] = origin[1] = -1;
        vLayer.clear(); 
        vBbox.clear(); 
	}
    /// @brief print data members 
    /// @param ss output stream 
	virtual void print(ostringstream& ss) const
	{
		ss << "//////// Port ////////" << endl
			<< "status = " << status << endl 
			<< "origin = " << origin[0] << " " << origin[1] << endl 
			<< "orient = " << orient << endl; 
        for (uint32_t i = 0; i < vLayer.size(); ++i)
		    ss << "layer " << vLayer[i] << " " << vBbox[i][0] << " " << vBbox[i][1] << " " << vBbox[i][2] << " " << vBbox[i][3] << endl;
	}
};
/// @brief pin of node/cell 
struct Pin : public Item
{
	string pin_name; ///< pin name 
	string net_name; ///< net name 
	string direct; ///< direction 
	string status; ///< placement status 
	int32_t origin[2]; ///< offset to node origin 
	string orient; ///< orientation 
    vector<string> vLayer; ///< layers  
    vector<vector<int32_t> > vBbox; ///< bounding box on each layer 
	string use; ///< "use" token in DEF file 
  vector<PinPort> vPinPort; ///< pin ports 
    /// @brief reset all data members 
	void reset()
	{
		pin_name = net_name = direct = status = orient = "";
		origin[0] = origin[1] = -1;
        vLayer.clear(); 
        vBbox.clear(); 
        vPinPort.clear();
		use = "";
	}
    /// @brief print data members 
    /// @param ss output stream 
	virtual void print(ostringstream& ss) const
	{
		ss << "//////// Pin ////////" << endl
			<< "pin_name = " << pin_name << endl 
			<< "net_name = " << net_name << endl 
			<< "direct = " << direct << endl 
			<< "status = " << status << endl 
			<< "origin = " << origin[0] << " " << origin[1] << endl 
			<< "orient = " << orient << endl; 
        for (uint32_t i = 0; i < vLayer.size(); ++i)
		    ss << "layer " << vLayer[i] << " " << vBbox[i][0] << " " << vBbox[i][1] << " " << vBbox[i][2] << " " << vBbox[i][3] << endl;
		ss << "use = " << use << endl;
    for (uint32_t i = 0; i < vPinPort.size(); ++i)
      vPinPort[i].print(ss); 
	}
};
/// @brief net to describe interconnection of netlist 
struct Net : public Item
{
	string net_name; ///< net name 
    int32_t net_weight; ///< net weight
	vector< std::pair<string, string> > vNetPin; ///< array of (node, pin) pair 
    float wirelength; ///< wirelength of the net
    /// @brief reset all data members 
	void reset()
	{
		net_name = "";
        net_weight = 1;
		vNetPin.clear();
	}
    /// @brief print data members 
    /// @param ss output stream 
	virtual void print(ostringstream& ss) const
	{
		ss << "//////// Net ////////" << endl
			<< "net_name = " << net_name << endl
            << "net_weight = " << net_weight << endl;
		for (uint32_t i = 0; i < vNetPin.size(); ++i)
			ss << "(" << vNetPin[i].first << ", " << vNetPin[i].second << ") ";
		ss << endl;
	}
};
/// @brief region to describe regions like fence 
struct Region : public Item
{
    string region_name; ///< region name 
    string region_type; ///< region type 
    std::vector<std::vector<int> > vRectangle; ///< array of rectangles with (xl, yl, xh, yh)
    std::vector<string> vPropertyName; ///< array of property names 
    std::vector<string> vPropertyValue; ///< array of property values 
    std::vector<char>  vPropertyType; ///< array of property types 

    /// @brief reset all data members 
    void reset()
    {
        region_name = "";
        region_type = "";
        vRectangle.clear();
        vPropertyName.clear();
        vPropertyValue.clear();
        vPropertyType.clear();
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Region ////////" << endl
			<< "region_name = " << region_name << endl
            << "region_type = " << region_type << endl;
        for (uint32_t i = 0; i < vRectangle.size(); ++i)
            ss << "(" << vRectangle[i][0] << ", " << vRectangle[i][1] << ", " << vRectangle[i][2] << ", " << vRectangle[i][3] << ") "; 
        ss << endl; 
        for (uint32_t i = 0; i < vPropertyName.size(); ++i)
            ss << "Property " << vPropertyName[i] << " " << vPropertyType[i] << " " << vPropertyValue[i] << endl; 
    }
};
/// @brief group to describe cells corresponding to region 
struct Group : public Item
{
    string group_name; ///< group name 
    std::vector<string> vGroupMember; ///< group members 
    string region_name; ///< region name 
    int perim; ///< MAXHALFPERIMETER, I do not know what they are used for 
    int maxx;
    int maxy; 
    std::vector<std::vector<int> > vRectangle; ///< array of regions with (xl, yl, xh, yh)
    std::vector<string> vPropertyName; ///< array of property names 
    std::vector<string> vPropertyValue; ///< array of property values 
    std::vector<char>  vPropertyType; ///< array of property types 

    /// @brief reset all data members 
    void reset()
    {
        group_name = "";
        vGroupMember.clear();
        region_name = "";
        perim = 0; 
        maxx = 0; 
        maxy = 0; 
        vRectangle.clear();
        vPropertyName.clear();
        vPropertyValue.clear();
        vPropertyType.clear();
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Group ////////" << endl
            << "group_name = " << group_name << endl
			<< "region_name = " << region_name << endl
            << "perim = " << perim << endl 
            << "maxx = " << maxx << ", " << "maxy = " << maxy << endl; 
        ss << "vGroupMember[" << vGroupMember.size() << "] = ";
        for (uint32_t i = 0; i < vGroupMember.size(); ++i)
            ss << vGroupMember[i] << " ";
        ss << endl; 
        for (uint32_t i = 0; i < vRectangle.size(); ++i)
            ss << "(" << vRectangle[i][0] << ", " << vRectangle[i][1] << ", " << vRectangle[i][2] << ", " << vRectangle[i][3] << ") "; 
        ss << endl; 
        for (uint32_t i = 0; i < vPropertyName.size(); ++i)
            ss << "Property " << vPropertyName[i] << " " << vPropertyType[i] << " " << vPropertyValue[i] << endl; 
    }
};
/// @brief routing tracks 
struct Track : public Item
{
    string track_name; ///< group name 
    std::vector<string> vLayerNames; ///< group members 
    int start;
    int step;
    int num; 
    int firstTrackMask;
    int sameMask; 

    /// @brief reset all data members 
    void reset()
    {
        track_name = "";
        vLayerNames.clear();
        start = 0;
        step = 0;
        num = 0;
        firstTrackMask = 0;
        sameMask = 0;
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Track ////////" << endl
            << "start = " << start << endl
            << "step  = " << step  << endl 
            << "num   = " << num   << endl
            << "Layer ";
        for (auto &layername: vLayerNames) {
            ss << layername << " ";
        }
        ss << endl;
    }
};
/// @brief routing tracks 
struct GCellGrid : public Item
{
    string gcellgrid_name; ///< group name  
    int start;
    int step;
    int num; 

    /// @brief reset all data members 
    void reset()
    {
        gcellgrid_name = "";
        start = 0;
        step = 0;
        num = 0;
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// GCellGrid ////////" << endl
            << "name  = " << gcellgrid_name << endl
            << "start = " << start << endl
            << "step  = " << step  << endl 
            << "num   = " << num   << endl;
    }
};
struct ViaType : public Item
{
    std::string viatype_name; ///< group name  
    int x, y;

    /// @brief reset all data members 
    void reset()
    {
        viatype_name = "";
        x = 0;
        y = 0;
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Via ////////" << endl
            << "type  = " << viatype_name << endl
            << "coord = ()" << x << ", " << y << ")" << endl;
    }
};
struct Via : public Item
{
    std::string viatype_name; ///< group name  
    int x, y;

    /// @brief reset all data members 
    void reset()
    {
        viatype_name = "";
        x = 0;
        y = 0;
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Via ////////" << endl
            << "type  = " << viatype_name << endl
            << "coord = ()" << x << ", " << y << ")" << endl;
    }
};
/// @brief routing tracks 
struct SNet : public Item
{
    std::string net_name; ///< group name  
    std::string type;
    vector<vector<int>> shapes; ///< shapes in snet, only support rectangles yet
    vector<Via> vias;
    

    /// @brief reset all data members 
    void reset()
    {
        net_name = "";
        shapes.clear();
        vias.clear();
        type = "";
    }
    /// @brief print data members 
    /// @param ss output stream 
    virtual void print(ostringstream& ss) const 
    {
		ss << "//////// Special Net ////////" << endl
            << "name  = " << net_name << endl
            << "type = " << type << endl
            << "vias = ";
        for (auto const& via: vias) {
            via.print(ss);
        }
        ss << endl << "shapes = ";
        for (auto const& rect: shapes) {
            ss << '(';
            for (int i = 0; i < rect.size(); ++i) {
                ss  << rect[i] << ", ";
            }
            ss << ") ";
        }
        ss << endl;
    }
};
// forward declaration
/// @class DefParser::DefDataBase
/// @brief Base class for def database. 
/// Only pure virtual functions are defined.  
/// User needs to inheritate this class and derive a custom database type with all callback functions defined.  
class DefDataBase
{
	public:
        /// @brief set divider characters 
		virtual void set_def_dividerchar(string const&) = 0;
        /// @brief set BUS bit characters 
		virtual void set_def_busbitchars(string const&) = 0;
        /// @brief set DEF version 
		virtual void set_def_version(string const&) = 0;
        /// @brief set design name 
		virtual void set_def_design(string const&) = 0;
        /// @brief set DEF unit 
		virtual void set_def_unit(int) = 0;
        /// @brief set die area xl, yl, xh, yh
		virtual void set_def_diearea(int, int, int, int) = 0;
        /// @brief set die area, optional parameters for non-rectangular shapes defined as points 
		virtual void set_def_diearea(int, const int*, const int*);
        /// @brief add row 
		virtual void add_def_row(Row const&) = 0;
        /// @brief add component/cell 
		virtual void add_def_component(Component const&) = 0;
        /// @brief add component/cell 
		virtual void resize_def_component(int) = 0;
        /// @brief add pin 
		virtual void add_def_pin(Pin const&) = 0;
        /// @brief set number of pins 
		virtual void resize_def_pin(int)  = 0;
        /// @brief add net 
		virtual void add_def_net(Net const&) = 0;
        /// @brief set number of nets 
		virtual void resize_def_net(int) = 0;
        ///
        virtual void add_def_track(defiTrack const&);
        virtual void add_def_gcellgrid(GCellGrid const&);
        virtual void add_def_snet(defiNet const&);
        virtual void add_def_via(defiVia const&);
        /// @brief add placement blockages, array of boxes with xl, yl, xh, yh
        virtual void add_def_route_blockage(std::vector<std::vector<int> > const&, std::string const&);
        // check these callbacks at runtime 
        /// @brief set number of blockages 
        virtual void resize_def_blockage(int);
        /// @brief add placement blockages, array of boxes with xl, yl, xh, yh
        virtual void add_def_placement_blockage(std::vector<std::vector<int> > const&);
        /// @brief set number of regions 
        virtual void resize_def_region(int); 
        /// @brief add region 
        virtual void add_def_region(Region const&); 
        /// @brief set number of groups 
        virtual void resize_def_group(int);
        /// @brief add group 
        virtual void add_def_group(Group const&); 
        /// @brief end of design 
        virtual void end_def_design(); 

    protected:
        /// @brief remind users to define some optional callback functions at runtime 
        /// @param str message including the information to the callback function in the reminder 
        void def_user_cbk_reminder(const char* str) const;
};

} // namespace DefParser

#endif
