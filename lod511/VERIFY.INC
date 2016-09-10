var
 verifyfail: boolean;

#define verify(x,y) \
         if (size(x)<>querylodint(y)) then \
          if ~verifyfail then \
           verifyfail:=true;  \
           writeln; \
          endif; \
          writeln("&CError: structure size mismatch, file=", \
                  #file, " line=",#linenum," struct=",#x); \
         endif;

procedure verifydatatypes;
begin;
 verifyfail:=false;
 verify(tpylonentry,querylod_sizetpylonentry);
 verify(tmapentry,querylod_sizetmapentry);
 verify(devdeftype,querylod_sizedevdeftype);
 verify(devicetype,querylod_sizedevicetype);
 verify(usertype,querylod_sizeusertype);
 verify(clonetype,querylod_sizeclonetype);
 verify(tmonindex,querylod_sizetmonindex);
 verify(terraintype,querylod_sizeterraintype);
 verify(baserec,querylod_sizebaserec);
 if verifyfail then
  writeln;
  writeln("&CYou may be running a dataset that is not compatible with your version");
  writeln("of LOD. Please contact the dataset author.");
  waitkey;
 endif;
end verifydatatypes;
