#!/usr/bin/env python

import sys
import os

from cpp import ast
from cpp import utils


FILE_H_HEADER="""
/**
This is automatically generate mock file (from {source_name}). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef {define_name}
#define {define_name}

#include "fff.h"
#include "{source_include}"

"""


FILE_H_FOOTER="""
#endif // {define_name}
"""

FILE_C_HEADER="""
/**
This is automatically generated mock file, see header file for more details.
*/


#include "{header_file}"
"""

FILE_C_FOOTER=""



class Mock_Generator:
   
   def __init__( self, fn_source, fn_target_h, fn_target_c ):
      self.fn_source = fn_source
      self.fn_target_h = fn_target_h
      self.fn_target_c = fn_target_c 
       
      self.fid_h = open( fn_target_h, 'w' )
      self.fid_c = open( fn_target_c, 'w' )
      
      self.print_formatted( self.fid_h, FILE_H_HEADER)
      self.print_formatted( self.fid_c, FILE_C_HEADER )
   
   
   def print_formatted( self, fid, string ):
      
      string_vars = { 'header_file' : os.path.basename( self.fn_target_h ),
                      'define_name' : "_AUTOMOCK_" + os.path.basename(self.fn_source).upper().replace(".","_"),
                      'source_name' : self.fn_source,
                      'source_include' : os.path.basename(self.fn_source)  }
      
      fid.write( string.format( **string_vars ) )
      
   def save(self):
      self.print_formatted( self.fid_h, FILE_H_FOOTER )
      self.print_formatted( self.fid_c, FILE_C_FOOTER )

      self.fid_h.close()
      self.fid_c.close()
   
   
   def _make_fff_fun( self, node_name, node, fun_par = None ):   
      fun_ret = process_type( node.return_type )
      
      if fun_par == None:
         fun_par = process_params( node )
      
      fake_params = [ ]
      
      if len(fun_par) > 0 and fun_par[-1][0] == "...":
         base_end="_VARARG"
      else:
         base_end=""
      
      if fun_ret == "void":
         base_type = "VOID" 
      else:
         base_type = "VALUE"
         fake_params.append( fun_ret )
      
      fake_params.append( node_name )
      for item in fun_par:
        fake_params.append( item[0] )
         
      fake_params = ", ".join(fake_params)
      base = "FAKE_%s_FUNC%s( %s );" % ( base_type, base_end, fake_params )
      self.fid_h.write( "DECLARE_" + base + "\n" )
      self.fid_c.write( "DEFINE_" + base + "\n" )
   
   def _make_class_fun( self, class_name, node ):
         fun_name = class_name + "__" + node.name
         
         fun_cls_name = node.name 
         fun_cls_mod = ""
         if node.modifiers == ast.FUNCTION_SPECIFIER: #FIXME: The plain cpp does not make difference with const, final etc modifiers, this will be a problem.
            fun_cls_mod = "const"
         elif node.modifiers == ast.FUNCTION_DTOR:
            fun_name += "_DTOR"
            fun_cls_name = "~" + fun_cls_name
            
         fun_params = [ ("%s %s*" % (fun_cls_mod, class_name) ,"this"),] + process_params( node )
         
         fun_fff_par_types  = ", ".join( [ x[0] for x in fun_params ] )
         fun_fff_par_names  = ", ".join( [ x[1] for x in fun_params ] )
         fun_fff_decl = "%s %s( %s )" % ( process_type( node.return_type ), fun_name , fun_fff_par_types )

         fun_cls_ret = process_type( node.return_type )
         
         if fun_cls_ret  == "void":
            if node.return_type == None:
               fun_cls_ret = ""
            fun_cls_ret_str = ""
         else:
            fun_cls_ret_str = "return "
         
         fun_cls_par_types = ", ".join( [ "%s %s" % (x[0],x[1]) for x in process_params( node ) ] )
         
            
         self.fid_h.write( fun_fff_decl + ";\n");
         
         self._make_fff_fun( fun_name, node, fun_par = fun_params  )
         self.fid_c.write( "%s %s::%s(%s) %s " % ( fun_cls_ret, class_name, fun_cls_name, fun_cls_par_types, fun_cls_mod ))
         self.fid_c.write( "{ %s%s(  %s ); }\n" % ( fun_cls_ret_str, fun_name, fun_fff_par_names ) )
         
      
   def run( self, ast_list ):
     for node in ast_list:
       if isinstance(node, ast.Function):
            self._make_fff_fun( node.name, node )
       elif isinstance(node, ast.Class) and node.body:
          class_name = node.name
          for child_node in node.body:
              if isinstance(child_node, ast.Function):
                  self._make_class_fun( class_name, child_node );                                   
                  
      
def process_params( node ):
    return [ ( process_type(par.type) ,  par.name ) for par in node.parameters ] 

def process_type( partype ):
    if partype == None:
       return "void"
    ret = "%s %s%s" % (" ".join(partype.modifiers), partype.full_name(), "*" if partype.pointer else "", )
    return ret.strip()

def fun_full( prefix, node ):
    params = fun_params( node ) 
    print "%sF:%s:%s(%s)" % ( prefix, process_type(node.return_type) , node.name, params )

      
def _GenerateMocks( ast_list, target_h, target_c  ):
  processed_class_names = set()
  lines = []
  
  for node in ast_list:
    if isinstance(node, ast.Function):
          fun_full("", node) 
    elif isinstance(node, ast.Class) and node.body:
          class_name = node.name
          parent_name = class_name

          print "C:%s:public:%s" % (class_name, parent_name )
          for child_node in node.body:
              #print child_node
              if isinstance(child_node, ast.Function):
                  fun_full("   ", child_node)        
#    import ipdb; ipdb.set_trace();
  return lines


def generate_mocks( filename_source, filename_h_target, filename_c_target  ):
  print "Generate fff mocks from :" + filename_source +"->" + filename_c_target
  try:
        source = utils.read_file(filename_source)
        if source is None:
            return

        builder = ast.builder_from_source( source, filename_source )
        entire_ast = list([_f for _f in builder.generate() if _f])
  except KeyboardInterrupt:
    return
  
  generator = Mock_Generator( filename_source, filename_h_target, filename_c_target )
  generator.run( entire_ast )
  generator.save()
  #generator.run( entire_ast )
  
  
if __name__ == "__main__":
   generate_mocks( sys.argv[1], sys.argv[2], sys.argv[3],)
         
